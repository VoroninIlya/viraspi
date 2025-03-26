#pragma once

#include "mongoose.h"

#include <string>
#include <vector>
#include <utility>

struct WebApiContext {

  WebApiContext() : connection(NULL), eventData(NULL) {}

  WebApiContext(mg_connection* con, int ev, void* evData) : 
    connection(con), event(ev), eventData(evData){}

  WebApiContext(const WebApiContext& obj) {
    swap(obj);
  }

  WebApiContext(WebApiContext&& obj) {
    swap(obj);
  }

  WebApiContext& operator=(const WebApiContext& obj) {
    if(this != &obj) {
      swap(obj);
    }
    return *this;
  }

  WebApiContext& operator=(WebApiContext&& obj) noexcept {
    if(this != &obj) {
      swap(obj);
    }
    return *this;
  }

  mg_connection* connection;
  void* eventData;
  int event;

private:
  void swap(const WebApiContext& obj) noexcept {
    this->connection = obj.connection;
    this->event = obj.event;
    this->eventData = obj.eventData;
  }
  
};