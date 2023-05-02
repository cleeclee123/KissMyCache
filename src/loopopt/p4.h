#define NUM_ANT_TYPES 16
#define DIRECTIONS 8
#define NUM_HILLS_RECORDED 81000

struct anthill {
  float longitude;
  float latitude;
  int tunnels[DIRECTIONS];
  int ant_count[NUM_ANT_TYPES];
  char loc_name[24];
};

struct anthill hills[NUM_HILLS_RECORDED];
int total_ants[NUM_ANT_TYPES];

int MIN(int a, int b) { return (b < a) ? b : a; }