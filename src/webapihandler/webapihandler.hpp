#pragma once

#include "viwebapi.hpp"
#include "viwebapinode.hpp"
#include "webapicontext.hpp"
#include <map>

using apiCtx = WebApiContext; // 
using api = viwebapi::ViWebApi<std::string, apiCtx&>; //
using node = viwebapi::ViWebApiNode<std::string, apiCtx&>; //

class WebApiHandler {

  WebApiHandler();

public:

  static WebApiHandler& getInstance() {
    static WebApiHandler ret;
    return ret;
  }

};