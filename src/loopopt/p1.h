float MIN(float a, float b) { return (b < a) ? b : a; }

#define NUM_PRESENTS 12000
#define NUM_STORES 7

struct item {
  float best_price;
  float prices[NUM_STORES];
};

struct item presents[NUM_PRESENTS];
