
#include "webapihandler.hpp"


// Route handling
//conn = c;
//if (mg_match(hm->uri, mg_str("/"), NULL)) {
//    serve_homepage(c);
//} else if (mg_match(hm->uri, mg_str("/about"), NULL)) {
//    serve_about(c);
//} else if (mg_match(hm->uri, mg_str("/contact"), NULL)) {
//    serve_contact(c);
//} else if (mg_match(hm->uri, mg_str("/api/status"), NULL)) {
//  std::string response = "{\"time\": \"" + getTime() + "\", \"cpuLoad\": \"" + getCpuLoad() + "\"}";
//  mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", response.c_str());
//} else if (mg_match(hm->uri, mg_str("/get-data"), NULL)) {
//      // Respond with dynamic data
//      printf("Synchronous result: %f\r\n",
//        VIHCSR04_MeasureDistance("HC-SR04 1", 19.307f, 400));
//      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"key\": \"%f\"}\0", measuredDistance);
//}
//else {
//    // Serve a 404 page for any undefined routes
//    mg_http_reply(c, 404, "Content-Type: text/html\r\n",
//                  "<html><body><h1>404 Not Found</h1>"
//                  "<p>The page you are looking for does not exist.</p>"
//                  "<p><a href=\"/\">Go back to homepage</a></p>"
//                  "</body></html>");
//}

//static void serve_homepage(struct mg_connection *c) {
//  mg_http_reply(c, 200, "Content-Type: text/html\r\n",
//                "<html><body><h1>Home</h1>"
//                "<p>This is the home page.</p>"
//                "</body></html>");
//}
//
//// Function to serve the about page
//static void serve_about(struct mg_connection *c) {
//  mg_http_reply(c, 200, "Content-Type: text/html\r\n",
//                "<html><body><h1>About Us</h1>"
//                "<p>This is the about page.</p>"
//                "<p><a href=\"/\">Go back to homepage</a></p>"
//                "</body></html>");
//}
//
//// Function to serve the contact page
//static void serve_contact(struct mg_connection *c) {
//  mg_http_reply(c, 200, "Content-Type: text/html\r\n",
//                "<html><body><h1>Contact Us</h1>"
//                "<p>This is the contact page.</p>"
//                "<p><a href=\"/\">Go back to homepage</a></p>"
//                "</body></html>");
//}