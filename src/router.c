#include <string.h>
#include <stdlib.h>

#include "router.h"

ri_Router* ri_router_new(ri_Handler default_handler) { 
  ri_Routes *routes;
  routes = (ri_Routes*) malloc(sizeof(routes));
  routes->first = NULL;
  routes->last = NULL;

  ri_Router *router;
  router = (ri_Router*) malloc(sizeof(router));
  router->routes = routes;
  router->default_handler = default_handler;

  return router;
}

ri_Route* ri_route_new(char *method, char *path, ri_Handler handler) {
  ri_Route *route;
  route = (ri_Route*) malloc(sizeof(route));
  route->path = path;
  route->method = (char*) malloc(16);
  strcpy(route->method, method);
  route->handler = handler;
  route->next = NULL;

  return route;
}

ri_Route* ri_get_route(ri_Router *router, char *path, ri_Handler handler) {
  ri_Route *route = ri_route_new("GET", path, handler);
  ri_add_route(router->routes, route);
  return route;
}

ri_Route* ri_post_route(ri_Router *router, char *path, ri_Handler handler) {
  ri_Route *route = ri_route_new("POST", path, handler);
  ri_add_route(router->routes, route);
  return route;
}

int ri_match_route(ri_Router *router, const char *method, const char *path, void *e) {
  ri_Route *r = router->routes->first;
  while (r != NULL) {
    if (strcmp(method, r->method) == 0 && strcmp(path, r->path) == 0) {
      return r->handler(e);
    }

    r = r->next;
  }

  return router->default_handler(e);
}

void ri_add_route(ri_Routes *routes, ri_Route *route) {
  if (routes->last != NULL) {
    routes->last->next = route;
    routes->last = route;
  } else {
    routes->first = route;
    routes->last = route;
  }
}

