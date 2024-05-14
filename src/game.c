
#include "game.h"

#define ROOM_MAX_SIZE 22
#define ROOM_MIN_SIZE 3

typedef enum door_dir_enum{
    D_TOP,
    D_RIGHT,
    D_LEFT,
    D_BOTTOM,
}Door_Direction;

Map_def get_inverse_door(Map_def direction){
    switch (direction)
    {
    case MAP_UNDISCOVERED_DOOR_NORTH:
        return MAP_UNDISCOVERED_DOOR_SOUTH;
    case MAP_UNDISCOVERED_DOOR_EAST:
        return MAP_UNDISCOVERED_DOOR_WEST;
    case MAP_UNDISCOVERED_DOOR_WEST:
        return MAP_UNDISCOVERED_DOOR_EAST;
    case MAP_UNDISCOVERED_DOOR_SOUTH:
        return MAP_UNDISCOVERED_DOOR_NORTH;
    default:
        ARA_error(VALUE_ERROR);
        return -1;
    }
}

Map_def discover_door(Map_def direction){
    switch (direction)
    {
    case MAP_UNDISCOVERED_DOOR_NORTH:
        return MAP_DOOR_NORTH;
    case MAP_UNDISCOVERED_DOOR_EAST:
        return MAP_DOOR_EAST;
    case MAP_UNDISCOVERED_DOOR_WEST:
        return MAP_DOOR_WEST;
    case MAP_UNDISCOVERED_DOOR_SOUTH:
        return MAP_DOOR_SOUTH;
    default:
        ARA_error(UNDEFINED_ERROR);
        return -1;
    }
}

boolean is_undiscovered_door(Map_def value){
    if (value >= MAP_UNDISCOVERED_DOOR_NORTH && value <= MAP_UNDISCOVERED_DOOR_SOUTH )
    {
        return 1;
    }
    return 0;
}

void generate_doors_on_wall(Game* game, int x1, int y1, int x2, int y2, Map_def direction){
    switch (direction)
    {
    case MAP_UNDISCOVERED_DOOR_NORTH:
        game->map.set(&game->map, randint(x1+1, x2-1), y1, MAP_UNDISCOVERED_DOOR_NORTH);
        break;
    case MAP_UNDISCOVERED_DOOR_EAST:
        game->map.set(&game->map, x2, randint(y1+1, y2-1), MAP_UNDISCOVERED_DOOR_EAST);
        break;
    case MAP_UNDISCOVERED_DOOR_WEST:
        game->map.set(&game->map, x1, randint(y1+1, y2-1), MAP_UNDISCOVERED_DOOR_WEST);
        break;
    case MAP_UNDISCOVERED_DOOR_SOUTH:
        game->map.set(&game->map, randint(x1+1, x2-1), y2, MAP_UNDISCOVERED_DOOR_SOUTH);
        break;
    default:
        break;
    }
}

void generate_doors(Game* game, int x1, int y1, int x2, int y2, Map_def banned_door){
     
    int probable_direction[3];
      
    int doors = randint(1,4);
    if (banned_door == MAP_NONE)
    {
        return;
    }
    if (banned_door == MAP_ALL || doors == 3){
        for (int direction = MAP_UNDISCOVERED_DOOR_NORTH; direction <= MAP_UNDISCOVERED_DOOR_SOUTH; direction++){   
            if (direction != (int)banned_door){
                generate_doors_on_wall(game, x1, y1, x2, y2, direction);
            }
            
            
        }
    } else {
         
        int index = 0;
        for (int direction = MAP_UNDISCOVERED_DOOR_NORTH; direction <= MAP_UNDISCOVERED_DOOR_SOUTH; direction++){   
            if (direction != (int)banned_door){
                probable_direction[index] = direction;
                index++;
            }
        }
        for (int i = 0; i < doors; i++){
              
            int rand_var = randint(0,3);
            generate_doors_on_wall(game, x1, y1, x2, y2, (Map_def)probable_direction[rand_var]);// regler pb
            
        }
          
        
    }
    
}


void fill_zone_and_doors(Game* game, int x1, int y1, int x2, int y2, Map_def banned_door){

    int max_x = max(x1,x2);
    int max_y = max(y1,y2);
    int min_x = min(x1,x2);
    int min_y = min(y1,y2);
     
    x1 = min_x;
    x2 = max_x;
    y1 = min_y;
    y2 = max_y;

     
    for (int y = y1; y < y2+1 ; y++)
    {
        game->map.get(&game->map, x1, y);
        game->map.get(&game->map, x2, y);
    }
     
    for (int x = x1; x < x2+1 ; x++)
    {   
        game->map.set(&game->map, x, y1, MAP_WALL);
        game->map.set(&game->map, x, y2, MAP_WALL);
    }
     
    for (int y = y1; y < y2+1 ; y++)
    {   
        game->map.set(&game->map, x1, y, MAP_WALL);
        game->map.set(&game->map, x2, y, MAP_WALL);
    }
     
    generate_doors(game, x1, y1, x2, y2, banned_door);
    
    
}

void generate_first_room(Game* game){
     
    int playerx = game->player.get_x(&game->player);
    int playery = game->player.get_y(&game->player);
    int size_x = randint(ROOM_MIN_SIZE*2, ROOM_MAX_SIZE-4);
    int size_y = randint(ROOM_MIN_SIZE*2, ROOM_MAX_SIZE-4);
    fill_zone_and_doors(game, playerx-size_x/2, playery-size_y/2,  playerx+size_x/2, playery+size_y/2, MAP_ALL);


}
void generatemap(Game* game){
     
    generate_first_room(game);
    
}

int is_block(Map_def value){
    return value < MAP_END_OF_BLOCK && value > MAP_ALL;
}

void generateroom(Game* game){
     
    int playerx = game->player.get_x(&game->player);
    int playery = game->player.get_y(&game->player);

    int x1, x2, y1, y2, get_x1, get_x2, get_y1, get_y2;
    int new_x1 = 0; 
    int new_x2;
    int new_y1; 
    int new_y2;
     
    int get_door = game->map.get(&game->map, playerx, playery);
      
    int size_x = randint(ROOM_MIN_SIZE, ROOM_MAX_SIZE);

    int size_y = randint(ROOM_MIN_SIZE, ROOM_MAX_SIZE);

    int offset_x = randint(2, size_x-1);

    int offset_y = randint(2, size_y-1);

    switch (get_door)
    {
    case MAP_UNDISCOVERED_DOOR_NORTH:
        x1 = playerx-offset_x;
        x2 = playerx+(size_x-offset_x);
        y1 = playery;
        y2 = playery-size_y;
        break;
    case MAP_UNDISCOVERED_DOOR_EAST:
        x1 = playerx;
        x2 = playerx+size_x;
        y1 = playery-offset_y;
        y2 = playery+(size_y-offset_y);
        break;
    case MAP_UNDISCOVERED_DOOR_WEST:
        x1 = playerx;
        x2 = playerx-size_x;
        y1 = playery-offset_y;
        y2 = playery+(size_y-offset_y);
        break;
    case MAP_UNDISCOVERED_DOOR_SOUTH:
        x1 = playerx-offset_x;
        x2 = playerx+(size_x-offset_x);
        y1 = playery;
        y2 = playery+size_y;
        break;
    default:
        break;
    }
    int max_x = max(x1,x2);
    int max_y = max(y1,y2);
    int min_x = min(x1,x2);
    int min_y = min(y1,y2);
     
    x1 = min_x;
    x2 = max_x;
    y1 = min_y;
    y2 = max_y;

    for (int x = x1+3; x < x2; x++)
    {   
        get_x1 = game->map.get(&game->map, x, y1);
        intlog(x);
        if (is_block(get_x1))
        {
            intlog(x);
            new_x1 = x + 1;
        }
        
    }

    x1 = new_x1;
    

    fill_zone_and_doors(game,x1, y1, x2, y2, get_inverse_door(get_door));
     
    game->map.set(&game->map, playerx, playery, discover_door(get_door));
    
}

void printmap(Game* game){
    int playerx = game->player.get_x(&game->player);
    int playery = game->player.get_y(&game->player);
    int offset_x = GAME_SCREEN_X/4;
    int offset_y = GAME_SCREEN_Y/2;

    int start_x = playerx - offset_x + 1;
    start_x = (start_x > 0) ? start_x : 0;

    int start_y = playery - offset_y + 1;
    start_y = (start_y > 0) ? start_y : 0;

    int end_x = playerx + offset_x;
    start_x = (end_x < MAP_SIZE_X) ? start_x : MAP_SIZE_X-GAME_SCREEN_X/2;

    int end_y = playery + offset_y ;
    start_y = (end_y < MAP_SIZE_Y) ? start_y : MAP_SIZE_Y-GAME_SCREEN_Y;

    int actual_x = start_x;
    int actual_y = start_y;
    int get_value;

    for (Size i = 1; i < GAME_SCREEN_X+1; i+=2)
    {   
        actual_y = start_y;
        for (Size j = 1; j < GAME_SCREEN_Y+1; j++)
        {   
            get_value = game->map.get(&game->map, actual_x, actual_y);
            
            if ( (playerx == actual_x && playery == actual_y))
            {   
                
                mvwprintw(game->window.top, j, i, "🤖");
                //mvwprintw(game->window.top, j, i, "Ƣ@");   
            }else
            {
                switch (get_value)
                {
                case MAP_WALL:
                    cprint(game->window.top, i, j, COLOR_MUR2, "🔳"); 
                    break;
                case MAP_UNDISCOVERED_DOOR_NORTH:
                case MAP_UNDISCOVERED_DOOR_EAST:
                case MAP_UNDISCOVERED_DOOR_WEST:
                case MAP_UNDISCOVERED_DOOR_SOUTH:
                case MAP_DOOR_NORTH:
                case MAP_DOOR_EAST:
                case MAP_DOOR_WEST:
                case MAP_DOOR_SOUTH:
                    cprint(game->window.top, i, j, COLOR_MUR2, "🌀"); 
                    
                    break;
                
                default:
                    break;
                }
                
            }
            
            
            actual_y++;
            
        }
        actual_x++;
        
    }
    
    
}

void check_generation(Game* game){
    int playerx = game->player.get_x(&game->player);
    int playery = game->player.get_y(&game->player);
    int get_value = game->map.get(&game->map, playerx, playery);
    if (is_undiscovered_door(get_value))
    {
        generateroom(game);
    }
    
}

boolean __check_move(Game* self, int x, int y){
    int sizex = self->map.sizex(&self->map);
    int sizey = self->map.sizey(&self->map);
    if (x>=0 && x<sizex && y>=0 && y<sizey && self->map.get(&self->map, x, y)!=MAP_WALL) {
        return 1;
    }
    return 0;
}


void __movement(Game* self){
    
    int x = self->player.get_x(&self->player);
    int y = self->player.get_y(&self->player);
    
    switch(self->window.get_key(&self->window)){
        case 'q':
            if(__check_move(self, x-1, y)){
                self->player.set_x(&self->player,x-1);
            }
            break;
        case 'd':
            if(__check_move(self, x+1, y)){
                self->player.set_x(&self->player,x+1);
            }
            break;
        case 'z' :
            if(__check_move(self, x, y-1)){
                self->player.set_y(&self->player,y-1);
            }
            break;
        case  's' :
            if(__check_move(self, x, y+1)){
                self->player.set_y(&self->player,y+1);
            }
    }
    self->window.__key = 0;
    check_generation(self);
}


void Game_init(Game* self){
    srand(time(NULL));
    log_reset();
    ARA_Window_init(&self->window, W_MODE_MULTIPLE);
    Map_init(&self->map, MAP_SIZE_X, MAP_SIZE_Y);
    Timer_init(&self->timer);
    Init_Player(&self->player);
}