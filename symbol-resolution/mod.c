extern int state;

static int increment;

int modify() {
  increment = state * 2;
  state += increment;
  return state;
}
