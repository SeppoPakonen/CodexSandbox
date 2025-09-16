#pragma once

// Internal header; aggregated by ManiaCore.h

struct HttpResponse {
    int    code = 0;
    String body;
    String error;
    
    /*
    HttpResponse() {}
    HttpResponse(const HttpResponse& r) {*this = r;}
    void operator=(const HttpResponse& r) {
        code = r.code;
        body = r.body;
        error = r.error;
    }
    */
};

HttpResponse HttpGet(const String& url, const VectorMap<String,String>* headers = nullptr, bool cache = false);
HttpResponse HttpPost(const String& url, const VectorMap<String,String>& fields, const VectorMap<String,String>* headers = nullptr, bool cache = false);

