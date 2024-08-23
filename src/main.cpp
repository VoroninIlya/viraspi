
#include <stdio.h>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
#include <thread>
#include <cstddef>
#include "pigpio.h"
#include "rotary_encoder.hpp"

#include "vihcsr04.h"
#include "vihcsr04.hpp"
#include "hcsr04callbacks/hcsr04callbacks.h"

#include "mongoose.h"

static float measuredDistance = 0.0f;

static struct mg_connection *conn = nullptr;

static void Distance(float distance) {
    measuredDistance = distance;
    printf("Distance: %f\r\n", distance);
  }

// Function to serve the homepage
static void serve_homepage(struct mg_connection *c) {
  // Generate HTML with embedded JavaScript

mg_http_reply(c, 200, "Content-Type: text/html\r\n",
  "<html lang='en'>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>Sensors Management Interface</title>"
    "<style>"
        "body {"
            "font-family: Arial, sans-serif;"
            "background-color: #f0f2f5;"
            "margin: 0;"
            "padding: 0;"
        "}"
        "header {"
            "background-color: #0044cc;"
            "color: white;"
            "padding: 20px;"
            "text-align: center;"
            "font-size: 1.5em;"
        "}"
        "nav {"
            "background-color: #0044cc;"
            "color: white;"
            "padding: 15px;"
            "display: flex;"
            "justify-content: space-around;"
            "align-items: center;"
        "}"
        "#nav li {"
          "float: left;"
          "list-style: none;"
        "}"
        "nav a {"
            "color: white;"
            "text-decoration: none;"
            "padding: 10px 20px;"
            "background-color: #0066ff;"
            "border-radius: 5px;"
            "transition: background-color 0.3s;"
        "}"
        "#nav li:last-child a {"
          "border-right: none;"
        "}"
        "nav a:hover {"
            "background-color: #003399;"
        "}"
        "#nav:after {"
          "content: ' ';"
          "display: table;"
          "clear: both;"
        "}"
        "#main-content {"
            "padding: 20px;"
            "margin: 10px;"
        "}"
        ".section {"
            "display: none;"
        "}"
        ".section.active {"
            "display: block;"
        "}"
        ".card {"
            "background-color: white;"
            "padding: 20px;"
            "margin: 20px 0;"
            "border-radius: 10px;"
            "box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);"
        "}"
        ".card h2 {"
            "margin-top: 0;"
        "}"
        ".form-group {"
            "margin: 15px 0;"
        "}"
        ".form-group label {"
            "display: block;"
            "margin-bottom: 5px;"
        "}"
        ".form-group input, .form-group select {"
            "width: 100%;"
            "padding: 10px;"
            "border: 1px solid #ccc;"
            "border-radius: 5px;"
        "}"
        ".button {"
            "padding: 10px 20px;"
            "background-color: #0044cc;"
            "color: white;"
            "border: none;"
            "border-radius: 5px;"
            "cursor: pointer;"
            "transition: background-color 0.3s;"
        "}"
        ".button:hover {"
            "background-color: #003399;"
        "}"
        "table {"
            "width: 100%;"
            "border-collapse: collapse;"
            "margin-bottom: 20px;"
        "}"
        "table, th, td {"
            "border: 1px solid #ccc;"
        "}"
        "th, td {"
            "padding: 10px;"
            "text-align: left;"
        "}"
        "th {"
            "background-color: #f2f2f2;"
        "}"
        "tr.selected {"
            "background-color: #cce5ff;"
        "}"
        ".actions {"
            "margin-bottom: 20px;"
            "text-align: right;"
        "}"
        ".actions button {"
            "padding: 10px 20px;"
            "margin-left: 10px;"
            "background-color: #0044cc;"
            "color: white;"
            "border: none;"
            "border-radius: 5px;"
            "cursor: pointer;"
        "}"
        ".actions button:hover {"
            "background-color: #003399;"
        "}"
    "</style>"
"</head>"
"<body>"
    "<header>"
      "Sensors Management Interface"
    "</header>"

    "<nav>"
      "<a href='#' onclick=showSection('dashboard')>Dashboard</a>"
      "<a href='#' onclick=showSection('sensors-settings')>Sensors Settings</a>"
      "<a href='#' onclick=showSection('connected-devices')>Connected Sensors</a>"
      "<a href='#' onclick=showSection('firmware-update')>Firmware Update</a>"
    "</nav>"

    "<div id='main-content'>"
        "<div id='dashboard' class='section active'>"
            "<div class='card'>"
                "<h2>Dashboard</h2>"
                "<p>Welcome to the router management interface. Use the navigation menu to access different sections.</p>"
            "</div>"
        "</div>"

        "<div id='sensors-settings' class='section'>"
            "<div class='card'>"
                "<h2>Sensors Settings</h2>"
                "<form>"
                "<div class='actions'>"
                    "<button onclick=showAddForm()>Add</button>"
                    "<button onclick=editSensor()>Edit</button>"
                    "<button onclick=deleteSensor()>Delete</button>"
                "</div>"

                "<table id='sensor-table'>"
                    "<thead>"
                        "<tr>"
                            "<th>Sensor Name</th>"
                            "<th>Type</th>"
                            "<th>Status</th>"
                            "<th>Enable/Disable</th>"
                        "</tr>"
                    "</thead>"
                    "<tbody>"
                        "<!-- Example row -->"
                        "<tr onclick=selectRow(this)>"
                            "<td>Temperature Sensor 1</td>"
                            "<td>Temperature</td>"
                            "<td>Enabled</td>"
                            "<td><input type='checkbox' checked></td>"
                        "</tr>"
                    "</tbody>"
                "</table>"

                "<!-- Add/Edit Form -->"
                "<div class='form-container' id='sensor-form'>"
                    "<div class='form-group'>"
                        "<label for='sensor-type'>Sensor Type:</label>"
                        "<select id='sensor-type' onchange=changeFormContent()>"
                            "<option value='temperature'>Temperature</option>"
                            "<option value='humidity'>Humidity</option>"
                            "<option value='pressure'>Pressure</option>"
                        "</select>"
                    "</div>"
                    "<div class='form-group'>"
                        "<label for='sensor-name'>Sensor Name:</label>"
                        "<input type='text' id='sensor-name' placeholder='Enter sensor name'>"
                    "</div>"
                    "<!-- Additional fields based on sensor type will be injected here -->"
                    "<div id='sensor-specific-fields'></div>"
                    "<div class='form-group'>"
                        "<button onclick='saveSensor()'>Save</button>"
                    "</div>"
                "</div>"

                "<script>"
                    "let selectedRow = null;"

                    "function selectRow(row) {"
                        "if (selectedRow) {"
                            "selectedRow.classList.remove('selected');"
                        "}"
                        "selectedRow = row;"
                        "row.classList.add('selected');"
                    "}"

                    "function showAddForm() {"
                        "selectedRow = null;" // Clear selection when adding a new sensor
                        "document.getElementById('sensor-form').style.display = 'block';"
                        "document.getElementById('sensor-name').value = '';"
                        "document.getElementById('sensor-type').value = 'temperature';"
                        "document.getElementById('sensor-specific-fields').innerHTML = '';"
                        "changeFormContent();"
                    "}"

                    "function changeFormContent() {"
                        "const sensorType = document.getElementById('sensor-type').value;"
                        "const fieldsContainer = document.getElementById('sensor-specific-fields');"
                        "fieldsContainer.innerHTML = '';"

                        "if (sensorType === 'temperature') {"
                            "fieldsContainer.innerHTML = `"
                                "<div class='form-group'>"
                                    "<label for='temp-range'>Temperature Range:</label>"
                                    "<input type='text' id='temp-range' placeholder='Enter temperature range'>"
                                "</div>"
                            "`;"
                        "} else if (sensorType === 'humidity') {"
                            "fieldsContainer.innerHTML = `"
                                "<div class='form-group'>"
                                    "<label for='humidity-range'>Humidity Range:</label>"
                                    "<input type='text' id='humidity-range' placeholder='Enter humidity range'>"
                                "</div>"
                            "`;"
                        "} else if (sensorType === 'pressure') {"
                            "fieldsContainer.innerHTML = `"
                                "<div class='form-group'>"
                                    "<label for='pressure-range'>Pressure Range:</label>"
                                    "<input type='text' id='pressure-range' placeholder='Enter pressure range'>"
                                "</div>"
                            "`;"
                        "}"
                    "}"

                    "function saveSensor() {"
                        "const table = document.getElementById('sensor-table').getElementsByTagName('tbody')[0];"
                        "const sensorName = document.getElementById('sensor-name').value;"
                        "const sensorType = document.getElementById('sensor-type').value;"
                        "const sensorStatus = 'Enabled';" // Default status

                        "if (selectedRow) {"
                            // Editing existing row
                            "selectedRow.cells[0].innerHTML = sensorName;"
                            "selectedRow.cells[1].innerHTML = sensorType.charAt(0).toUpperCase() + sensorType.slice(1);"
                        "} else {"
                            // Adding new row
                            "const row = table.insertRow();"
                            "row.onclick = function() { selectRow(row); };"
                            "const cell1 = row.insertCell(0);"
                            "const cell2 = row.insertCell(1);"
                            "const cell3 = row.insertCell(2);"
                            "const cell4 = row.insertCell(3);"

                            "cell1.innerHTML = sensorName;"
                            "cell2.innerHTML = sensorType.charAt(0).toUpperCase() + sensorType.slice(1);"
                            "cell3.innerHTML = sensorStatus;"
                            "cell4.innerHTML = `<input type='checkbox' checked>`;"
                        "}"

                        "document.getElementById('sensor-form').style.display = 'none';"
                    "}"

                    "function editSensor() {"
                        "if (selectedRow) {"
                            "document.getElementById('sensor-form').style.display = 'block';"
                            "document.getElementById('sensor-name').value = selectedRow.cells[0].innerHTML;"
                            "document.getElementById('sensor-type').value = selectedRow.cells[1].innerHTML.toLowerCase();"
                            "changeFormContent();" // Adjust the form content based on the sensor type
                        "} else {"
                            "alert('Please select a sensor to edit.');"
                        "}"
                    "}"

                    "function deleteSensor() {"
                        "if (selectedRow) {"
                            "selectedRow.remove();"
                            "selectedRow = null;"
                        "} else {"
                            "alert('Please select a sensor to delete.');"
                        "}"
                    "}"
                "</script>"
                "</form>"
            "</div>"
        "</div>"

        "<div id='connected-devices' class='section'>"
            "<div class='card'>"
                "<h2>Connected Devices</h2>"
                "<ul id='device-list'>"
                    "<li>Device 1: 192.168.0.2</li>"
                    "<li>Device 2: 192.168.0.3</li>"
                    "<li>Device 3: 192.168.0.4</li>"
                "</ul>"
            "</div>"
        "</div>"

        "<div id='firmware-update' class='section'>"
            "<div class='card'>"
                "<h2>Firmware Update</h2>"
                "<form>"
                    "<div class='form-group'>"
                        "<label for='firmware'>Upload Firmware:</label>"
                        "<input type='file' id='firmware' name='firmware'>"
                    "</div>"
                    "<button type='button' class='button' onclick='updateFirmware()'>Update Firmware</button>"
                "</form>"
            "</div>"
        "</div>"
    "</div>"

    "<script>"
        "function showSection(sectionId) {"
            "const sections = document.querySelectorAll('.section');"
            "sections.forEach(section => {"
                "section.classList.remove('active');"
            "});"
            "document.getElementById(sectionId).classList.add('active');"
        "}"
        "function saveNetworkSettings() {"
            "alert('Network settings saved!');"
            // Add logic to send the settings to the server
        "}"

        "function updateFirmware() {"
            "alert('Firmware update initiated!');"
            // Add logic to handle firmware upload and update
        "}"
    "</script>"
"</body>"
"</html>" );

  //mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  //mg_printf(c, "<html><body>");
  //mg_printf(c, "<div id='dynamic-content'>Initial Content</div>");
  //mg_printf(c, "<script>"
  //             "function updateContent() {"
  //             "  var xhr = new XMLHttpRequest();"
  //             "  xhr.open('GET', '/get-data', true);"
  //             "  xhr.onreadystatechange = function() {"
  //             "    if (xhr.readyState == 4 && xhr.status == 200) {"
  //             "      document.getElementById('dynamic-content').innerText = JSON.parse(xhr.responseText).key;"
  //             "    }"
  //             "  };"
  //             "  xhr.send();"
  //             "}"
  //             "setInterval(updateContent, 1000);"
  //             "</script>");
  //mg_printf(c, "<p><a href=\"/about\">About Us</a></p>");
  //mg_printf(c, "<p><a href=\"/contact\">Contact Us</a></p>");
  //mg_printf(c, "</body></html>");
}

// Function to serve the about page
static void serve_about(struct mg_connection *c) {
    mg_http_reply(c, 200, "Content-Type: text/html\r\n",
                  "<html><body><h1>About Us</h1>"
                  "<p>This is the about page.</p>"
                  "<p><a href=\"/\">Go back to homepage</a></p>"
                  "</body></html>");
}

// Function to serve the contact page
static void serve_contact(struct mg_connection *c) {
    mg_http_reply(c, 200, "Content-Type: text/html\r\n",
                  "<html><body><h1>Contact Us</h1>"
                  "<p>This is the contact page.</p>"
                  "<p><a href=\"/\">Go back to homepage</a></p>"
                  "</body></html>");
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
      struct mg_http_message *hm = (struct mg_http_message *) ev_data;

      // Route handling
      conn = c;
      if (mg_match(hm->uri, mg_str("/"), NULL)) {
          serve_homepage(c);
      } else if (mg_match(hm->uri, mg_str("/about"), NULL)) {
          serve_about(c);
      } else if (mg_match(hm->uri, mg_str("/contact"), NULL)) {
          serve_contact(c);
      }
      else if (mg_match(hm->uri, mg_str("/get-data"), NULL)) {
            // Respond with dynamic data
            printf("Synchronous result: %f\r\n",
              VIHCSR04_MeasureDistance("HC-SR04 1", 19.307f, 400));
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"key\": \"%f\"}\0", measuredDistance);
      }
      else {
          // Serve a 404 page for any undefined routes
          mg_http_reply(c, 404, "Content-Type: text/html\r\n",
                        "<html><body><h1>404 Not Found</h1>"
                        "<p>The page you are looking for does not exist.</p>"
                        "<p><a href=\"/\">Go back to homepage</a></p>"
                        "</body></html>");
      }

      //struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
      //mg_http_serve_dir(c, hm, &opts);
    }
}

static void EncoderCb(int v){
   printf("%d\r\n", v);
}

static void DelayMs(uint32_t ms);

int main(void) {
  printf("Hallo world\r\n");

  if (gpioInitialise() < 0) {
    printf("Error by gpio initialization\r\n");
    return 0;
  }

  re_decoder dec{22,23,&EncoderCb};

  gpioSetMode(6, PI_OUTPUT);
  gpioSetPullUpDown(6, PI_PUD_DOWN);
  gpioSetMode(5, PI_INPUT);
  gpioSetPullUpDown(6, PI_PUD_DOWN);

  //vihcsr04::Hcsr04Callbacks hcsr04Cb{};

  struct mg_mgr mgr;  // Declare event manager
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, nullptr);  // Setup listener

  // c usage
  VIHCSR04_SetPrintfCb((VIHCSR04_Printf_t)printf);
  VIHCSR04_SetDebugLvl(VIHCSR04_DEBUG_INFO);
  VIHCSR04_Init(vihcsr04::Hcsr04Callbacks::PulseIn, vihcsr04::Hcsr04Callbacks::TriggerPort);
  VIHCSR04_Create("HC-SR04 1", nullptr, 6, nullptr, 5);

  //VIHCSR04_MeasureDistanceAsync("HC-SR04 1", VIHCSR04_CONTINUOUS_MEASURE, 
  // 31.307f, 400, (VIHCSR04_Distance_t)Distance, &hcsr04Cb);

  // c++ usage
  //vihcsr04::Hcsr04Sensor sensor{vihcsr04::Hcsr04Callbacks::PulseIn, vihcsr04::Hcsr04Callbacks::TriggerPort};   
  //sensor.SetDebugLvl(vihcsr04::DebugLvl_t::DEBUG_INFO);
  //sensor.SetPrintfCb((vihcsr04::Printf_t)printf);
  //sensor.AddSensor("HC-SR04 2", nullptr, 6, nullptr, 5);

  //sensor.MeasureDistanceAsync("HC-SR04 2", vihcsr04::MeasureMode_t::CONTINUOUS_MEASURE, 
  //31.307f, 400, (vihcsr04::Distance_t)Distance, &hcsr04Cb);

  while(1) {

    //VIHCSR04_Runtime(); // c runtime
    //DelayMs(100);

    //sensor.Runtime(); // c++ runtime
    //DelayMs(100);

    //printf("Synchronous result: %f\r\n",
    //  VIHCSR04_MeasureDistance("HC-SR04 1", 19.307f, 400));
    //DelayMs(1000);

    //printf("Synchronous result: %f\r\n",
    //  sensor.MeasureDistance("HC-SR04 2", 19.307f, 400));
    //DelayMs(100);

    mg_mgr_poll(&mgr, 1000);
  }

  gpioTerminate();

  return 0;
}

__attribute__((weak)) int Reset_Handler() {
   return 0;
}

void DelayMs(uint32_t ms) {
   struct timespec ts;
    ts.tv_sec = ms / 1000; 
    ts.tv_nsec = ms % 1000 * 1000000L;   

   nanosleep(&ts, nullptr);  // Sleep for 1 millisecond
}