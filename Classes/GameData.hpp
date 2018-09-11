/*template < typename T >
T GameData::GetValue( EGameDataSheet sheet, unsigned index, std::string key )
{
    CCLog("Un-support Gamedata type.");
    XAssert(false, "");
}

template<>
float GameData::GetValue( EGameDataSheet sheet, unsigned index, std::string key )
{
    cJSON* pJsonRoot = GetJsonByType(sheet);

    cJSON* pStructJson = cJSON_GetArrayItem(pJsonRoot, index);
    if (!pStructJson)
        return 0.0f;

    cJSON* pValueJson = cJSON_GetObjectItem(pStructJson, key.c_str());
    if (!pValueJson)
        return 0.0f;

    return pValueJson->valuedouble;
}

template<>
std::string GameData::GetValue(EGameDataSheet sheet, unsigned index, std::string key)
{
    cJSON* pJsonRoot = GetJsonByType(sheet);

    cJSON* pStructJson = cJSON_GetArrayItem(pJsonRoot, index);
    if (!pStructJson)
        return "";

    cJSON* pValueJson = cJSON_GetObjectItem(pStructJson, key.c_str());
    if (!pValueJson)
        return "";

    return pValueJson->valuestring;
}*/