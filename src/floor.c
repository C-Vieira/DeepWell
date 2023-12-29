#include <rogue.h>

int selectEnemiesFromPool(void) {
    int type;

    switch (floorCount)
    {
    case 1:
    case 2:
        type = FLY; //just flies
        break;
    case 3:
    case 4:
        type = (rand() % 3) + 1; //flies, worms or rats
        break;
    case 5:
        type = (rand() % 3) + 2; //worms, rats or rat king
        break;
    case 6:
    case 7:
        type = (rand() % 2) + 6; //cave spiders or bats
        break;
    case 8:
    case 9:
        type = (rand() % 3) + 6; //cave spiders, bats or ghouls
        break;
    case 10:
        type = (rand() % 3) + 7; //bats, ghouls or centipede
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        type = (rand() % 2) + 10; //deep one or ooze
        break;
    case 15:
        type = (rand() % 3) + 10; //deep one, ooze or dagon
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        type = (rand() % 2) + 13; //star spawn or cosmic horror
        break;
    default:
        type = EMPTY;
        break;
    }

    return type;
}

char* selectFloorName(void) {
    switch (floorCount)
    {
    case 0:
        return "THE WELL";
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return "THE UNDERGROWTH";
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return "THE DEPTHS";
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return "Y'HA-NTHLEI OUTSKIRTS";
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return "SUNKEN CITY OF R'LYEH";
        break;
    case 21:
        return "SUNKEN KING'S LAIR";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

void showFloorMessage(void) {
    switch (floorCount)
    {
    case 0:
        mvprintw(0, 0, "The door is shut, it's only down from here ...");
        getch();
        break;
    case 1:
        mvprintw(0, 0, "You can hear flies buzzing about ...");
        getch();
        break;
    case 6:
        mvprintw(0, 0, "A chill runs down your spine ...");
        getch();
        break;
    case 11:
        mvprintw(0, 0, "You're beyond return ...");
        getch();
        break;
    case 16:
        mvprintw(0, 0, "We were not meant to get here ...");
        getch();
        break;
    case 21:
        mvprintw(0, 0, "Chaos incarnate Cthulhu awakens !");
        getch();
        break;
    default:
        break;
    }
}
