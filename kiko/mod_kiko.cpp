#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <apr_strings.h>
#include <regex>
#include "predicat.h"

extern "C" {

std::map<std::string, std::string> table_to_map(const apr_table_t* table, request_rec *r) {
    std::map<std::string, std::string> result;

    result["Method"] = r->method;
    result["URI"] = r->uri;
    result["Protocol"] = r->protocol;
    result["IP"] = r->connection->client_ip; 
    //result["Date et Heure"] = get_current_datetime();

    const apr_array_header_t* headers = apr_table_elts(table);
    apr_table_entry_t* entries = (apr_table_entry_t*) headers->elts;

    for (int i = 0; i < headers->nelts; ++i) {
        result[entries[i].key] = entries[i].val;
    }
    return result;
}

int verify_request(request_rec *r) {
    
    std::vector<std::pair<std::string, std::regex> > predicates = {
        {"IP", std::regex("^127\\.0\\.0\\.1$")},
        {"Method", std::regex("^(GET|POST|DELETE|PUT)$")},
        {"Protocol", std::regex("^HTTP/\\d\\.\\d$")},
        {"URI", std::regex("^/website/(index\\.html|audit\\.html)$")},
        //{std::regex("^Time$"), std::regex("^(0[8-9]|1[0-6]):[0-5][0-9]-((1[0-7]):[0-5][0-9]|18:00)$")},
    };

    auto headers_map = table_to_map(r->headers_in, r);
    bool all_conditions_met = true;
    for (auto& [key, regex] : predicates) {
        auto header = headers_map.find(key);
        if (!std::regex_match(header->second, regex)) {
            return DECLINED;
        }
    }
    
    return OK; 
}

std::vector<char> stringToVector(const std::string& str) {
    // Initialiser un vector de char avec la taille de str
    std::vector<char> vec(str.begin(), str.end());
    return vec;
}

static int kiko_handler(request_rec *r)
{
    // Création d'un automate pour reconnaître la séquence "127.0.0.1"
    //Automate<char> IP({'1','2','7','.','0','.','0','.','1'});
    // Création d'un automate pour reconnaître la séquence "GET|POST|DELETE|PUT"
    //Automate<char> method({'G','E','T','|','P','O','S','T','|','D','E','L','E','T','E','|','P','U','T'});
    //Automate<char> ress({'1','2','7','.','0','.','0','.','1'});

    std::map<std::string, std::string> request_info;
    request_info["Method"] = r->method;
    request_info["URI"] = r->unparsed_uri;
    request_info["Protocol"] = r->protocol;
    request_info["IP"] = r->connection->client_ip;
    
    if (strcmp(r->handler, "kiko")) {
        return DECLINED;
    }
    
    if(verify_request(r) == OK){
        return DECLINED;
        
    } else {
        ap_set_content_type(r, "text/html");
        ap_rputs("<html><head><title>Accès Refusé</title></head><body>", r);
        ap_rputs("<h1>403 Accès Refusé</h1>", r);
        ap_rputs("<p>Vous n'avez pas la permission d'accéder à cette ressource.</p>", r);
        ap_rputs("</body></html>", r);
        //return HTTP_FORBIDDEN;
    }

    return OK;
}



static void kiko_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(kiko_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

}



module AP_MODULE_DECLARE_DATA kiko_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    kiko_register_hooks  
};

