
#include <pif_plugin.h>

int pif_plugin_filter_func(EXTRACTED_HEADERS_T *headers, MATCH_DATA_T *data)
{
    return PIF_PLUGIN_RETURN_DROP;

}
