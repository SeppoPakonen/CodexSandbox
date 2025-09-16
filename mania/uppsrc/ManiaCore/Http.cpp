#include "ManiaCore.h"

NAMESPACE_UPP

static String CacheDir()
{
    String d = ConfigFile("ManiaCoreCache");
    RealizeDirectory(d);
    return d;
}

static String CachePath(const String& key)
{
    return AppendFileName(CacheDir(), key + ".cache");
}

static String MakeCacheKey(const String& method, const String& url, const VectorMap<String,String>* fields)
{
    String s = method + "\n" + url + "\n";
    if (fields) {
        Vector<String> ks;
        for (int i = 0; i < fields->GetCount(); ++i) ks.Add(fields->GetKey(i));
        Sort(ks);
        for (const String& k : ks) {
            s << k << '=' << fields->Get(k) << '\n';
        }
    }
    return MD5Hex(s);
}

static void ApplyHeaders(HttpRequest& r, const VectorMap<String,String>* headers)
{
    if (!headers) return;
    for (int i = 0; i < headers->GetCount(); ++i)
        r.Header(headers->GetKey(i), headers->GetValue(i));
}

HttpResponse HttpGet(const String& url, const VectorMap<String,String>* headers, bool cache)
{
    HttpResponse resp;
    String key = MakeCacheKey("GET", url, nullptr);
    if (cache) {
        String path = CachePath(key);
        if (FileExists(path)) {
            resp.body = LoadFile(path);
            resp.code = 200;
            return resp;
        }
    }
    HttpRequest r(url);
    ApplyHeaders(r, headers);
    String body = r.Execute();
    resp.code = r.GetStatusCode();
    if (resp.code >= 200 && resp.code < 300) {
        resp.body = body;
        if (cache) SaveFile(CachePath(key), body);
    } else {
        resp.error = r.GetErrorDesc();
        if (resp.error.IsEmpty()) resp.error = Format("HTTP error %d", resp.code);
    }
    return resp;
}

HttpResponse HttpPost(const String& url, const VectorMap<String,String>& fields, const VectorMap<String,String>* headers, bool cache)
{
    HttpResponse resp;
    String key = MakeCacheKey("POST", url, &fields);
    if (cache) {
        String path = CachePath(key);
        if (FileExists(path)) {
            resp.body = LoadFile(path);
            resp.code = 200;
            return resp;
        }
    }
    HttpRequest r(url);
    for (int i = 0; i < fields.GetCount(); ++i)
        r.Post(fields.GetKey(i), fields[i]);
    ApplyHeaders(r, headers);
    String body = r.Execute();
    resp.code = r.GetStatusCode();
    if (resp.code >= 200 && resp.code < 300) {
        resp.body = body;
        if (cache) SaveFile(CachePath(key), body);
    } else {
        resp.error = r.GetErrorDesc();
        if (resp.error.IsEmpty()) resp.error = Format("HTTP error %d", resp.code);
    }
    return resp;
}

END_UPP_NAMESPACE

