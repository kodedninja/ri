#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sandbird.h"
#include "ri.h"

static int index_handler(void *payload) {
  sb_Event *e = (sb_Event*) payload;
  /* Send page */
  sb_send_status(e->stream, 200, "OK");
  sb_send_header(e->stream, "Content-Type", "text/html");
  sb_writef(e->stream,
            "<!DOCTYPE html>"
            "<html><body>q = ''<br><br>"
            "<form method='post'><input type='text' name='q'></form>"
            "</body></html>");
  
  return SB_RES_OK;
}

static int index_post_handler(void *payload) {
  sb_Event *e = (sb_Event*) payload;
  char buf[512], *p;

  /* Get var and replace non-alpha numeric / space chars with underscore */
  sb_get_var(e->stream, "q", buf, sizeof(buf));
  for (p = buf; *p; p++) {
    if ( !(isspace(*p) || isalnum(*p)) ) {
      *p = '_';
    }
  }
  
  /* Send page */
  sb_send_status(e->stream, 200, "OK");
  sb_send_header(e->stream, "Content-Type", "text/html");
  sb_writef(e->stream,
            "<!DOCTYPE html>"
            "<html><body>q = '%s'<br><br>"
            "<form method='post'><input type='text' name='q'></form>"
            "</body></html>", buf);
  
  return SB_RES_OK;
}

static int test_handler(void *payload) {
  sb_Event *e = (sb_Event*) payload;
  sb_send_status(e->stream, 200, "OK");
  sb_send_header(e->stream, "Content-Type", "text/html");
  sb_writef(e->stream,
            "<!DOCTYPE html>"
            "<html><body>test page"
            "</body></html>");

  return SB_RES_OK;
}

static int not_found_handler(void *payload) {
  sb_Event *e = (sb_Event*) payload;
  sb_send_status(e->stream, 404, "OK");
  sb_send_header(e->stream, "Content-Type", "text/html");
  sb_writef(e->stream,
            "<!DOCTYPE html>"
            "<html><body>not found"
            "</body></html>"
  );

  return SB_RES_OK;
}

int route_handler(sb_Event *e) {
  if (e->type == SB_EV_REQUEST) {
    printf("%s - %s %s\n", e->address, e->method, e->path);

    ri_Router *router = e->udata;
    return ri_match_route(router, e->method, e->path, e);
  }
  
  return SB_RES_OK;
}

int main(void) {
  sb_Options opt;
  sb_Server *server;

  ri_Router *router = ri_router_new(not_found_handler);
  ri_get_route(router, "/", index_handler);
  ri_post_route(router, "/", index_post_handler);
  ri_get_route(router, "/test", test_handler);

  memset(&opt, 0, sizeof(opt));
  opt.port = "8000";
  opt.handler = route_handler;
  opt.udata = router;

  server = sb_new_server(&opt);

  if (!server) {
    fprintf(stderr, "failed to initialize server\n");
    exit(EXIT_FAILURE);
  }

  printf("Server running at http://localhost:%s\n", opt.port);

  for (;;) {
    sb_poll_server(server, 1000);
  }

  sb_close_server(server);
  return EXIT_SUCCESS;
}
