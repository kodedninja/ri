#ifndef ROUTER_H
#define ROUTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ri_Route ri_Route;
typedef struct ri_Routes ri_Routes;
typedef struct ri_Router ri_Router;
typedef int (*ri_Handler)(void*);

struct ri_Route {
  char *path;
  char *method;
  ri_Handler handler;
  struct ri_Route *next;
};

struct ri_Routes {
  ri_Route *first;
  ri_Route *last;
};

struct ri_Router {
  ri_Routes *routes;
  ri_Handler default_handler;
};

ri_Router* ri_router_new(ri_Handler default_handler);
ri_Route* ri_route_new(char *method, char *path, ri_Handler handler);
void ri_add_route(ri_Routes *routes, ri_Route *route);
ri_Route* ri_get_route(ri_Router *router, char *path, ri_Handler handler);
ri_Route* ri_post_route(ri_Router *router, char *path, ri_Handler handler);
int ri_match_route(ri_Router *router, const char *method, const char *path, void *e);

#ifdef __cplusplus
}
#endif

#endif

