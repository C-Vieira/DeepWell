#include <rogue.h>

//add enemy to the front of global entity list
void addEnemy(Entity* enemy){
    EntityList* newEntry = calloc(1, sizeof(EntityList));
    newEntry->entity = enemy;
    newEntry->next = enemies;

    enemies = newEntry;
}

//empty out the global entity list
void clearAllEnemies(void){
    EntityList* p = enemies;
    while(enemies != NULL){
        enemies = enemies->next;
        free(p);
        p = enemies;
    }
}

//remove a given enemy from the global entity list
void removeEnemy(Entity* target){
    if(enemies->entity == target || (enemies->entity == target && enemies->next == NULL)){ //found at first entry
        EntityList* p = enemies;
        enemies = enemies->next;
        free(p);
        return;
    }

    EntityList* aux = enemies;
    while(aux->next != NULL){
        if(aux->next->entity == target){
            EntityList* p = aux->next;
            aux->next = aux->next->next;
            free(p);
            return;
        }
        aux = aux->next;
    }
    return;
}

int pickEnemyColorFromType(int type) {
    switch (type)
    {
    case FLY:
    case RAT:
    case RAT_KING:
    case CAVE_SPIDER:
    case BAT:
    case GHOUL:
        return COLOR_PAIR(VISIBLE_COLOR);
        break;
    case WORM:
    case CENTIPEDE:
    case STAR_SPAWN:
        return COLOR_PAIR(YELLOW_BLACK);
        break;
    case CORPSE:
    case CTHULHU:
        return COLOR_PAIR(RED_BLACK);
        break;
    case OOZE:
        return COLOR_PAIR(GREEN_BLACK);
        break;
    case DEEP_ONE:
    case DAGON:
        return COLOR_PAIR(CYAN_BLACK);
        break;
    case LIVING_FLESH:
    case COSMIC_HORROR:
        return COLOR_PAIR(MAGENTA_BLACK);
        break;
    case EMPTY:
        return COLOR_PAIR(BLACK_BLACK);
        break;
    default:
        return COLOR_PAIR(VISIBLE_COLOR);
        break;
    }
}

//deal damage to a given target based on attacker's damage stat and target's defense stat
void attackEntity(Entity* target, Entity* attacker){
    if (target->health <= 0 && target->type == PLAYER) return; //skip if the player has already died this turn

    if(target->defense >= attacker->damage){ //target's defense is too high, cannot take any damage
        attron(COLOR_PAIR(VISIBLE_COLOR));
        mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
        mvprintw(26, 75, "%s blocked the attack!", target->name); //log message
        attroff(COLOR_PAIR(VISIBLE_COLOR));
        getch();
        return;
    }

    target->health -= (attacker->damage - target->defense);

    attron(COLOR_PAIR(VISIBLE_COLOR));
    mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
    mvprintw(26, 75, "%s scored a hit on %s!", attacker->name, target->name); //log message
    attroff(COLOR_PAIR(VISIBLE_COLOR));
    getch();
    if(target->health <= 0){ //spawn corpse on enemy death
        spawnCorpse(target);
        attacker->xpAmount += target->xpAmount;
        attron(COLOR_PAIR(VISIBLE_COLOR));
        mvprintw(26, 75, "                                                    "); //there must be a better way to clear a line...
        mvprintw(26, 75, "%s was defeated!", target->name); //log message
        attroff(COLOR_PAIR(VISIBLE_COLOR));
        getch();

        if (target->type == PLAYER) drawDefeatScreen(attacker->name); //if it was a player, end the game
    }
}

void spawnCorpse(Entity* entity){
    if (entity->ch == '%') { //living flesh
        removeEnemy(entity);
        return;
    }else if (entity->ch == 'C') { //final boss
        removeEnemy(entity);
        defeatedFinalBoss = true;
        return;
    }

    int chanceForLivingFlesh = rand() % 10; //1 in 10 - 10%
    if((chanceForLivingFlesh != 1))
        addEnemy(createEnemy(entity->pos, CORPSE, "Corpse", '%', COLOR_PAIR(RED_BLACK), 0, 0, 0, true, 0)); //add a corpse entity to the global entity list
    else
        addEnemy(createEnemy(entity->pos, LIVING_FLESH, "Living Flesh", '%', COLOR_PAIR(RED_BLACK), (entity->health + 5), 8, 3, false, 20)); //add living flesh instead
    removeEnemy(entity); //remove dead enemy from the list
}

Entity* createEnemy(Position pos, int type, char* name, char symbol, int color, int health, int damage, int defense,bool isImmobile, int xpAmount){

    Entity* newEnemy = calloc(1, sizeof(Entity));

    newEnemy->pos.y = pos.y;
    newEnemy->pos.x = pos.x;
    newEnemy->type = type;
    newEnemy->name = name;
    newEnemy->ch = symbol;
    newEnemy->color = color;
    newEnemy->health = health;
    newEnemy->damage = damage;
    newEnemy->defense = defense;
    newEnemy->immobile = isImmobile;
    newEnemy->seen = false;
    newEnemy->level = 0;
    newEnemy->xpAmount = xpAmount;
    newEnemy->amountForNextLevel = 0;

    return newEnemy;
}

//check for a target at a given position
Entity* checkForTarget(Position target_pos){
    EntityList* p = enemies;

    while(p != NULL){ //search the entity list for a target at the given position
        if((p->entity->pos.y == target_pos.y) && (p->entity->pos.x == target_pos.x) && !(p->entity->immobile))
            return p->entity;
        p = p->next; //found a target
    }

    return NULL; //no target found
}

void moveEnemies(void){
    EntityList* p = enemies;
    while(p != NULL){
        //followTarget(p->entity, player); //move all enemies
        seekTarget(p->entity, player);
        p = p->next;
    }
}
