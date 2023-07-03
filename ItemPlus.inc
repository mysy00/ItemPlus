// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_ItemPlus
#undef _inc_ItemPlus
#endif

#if defined _ItemPlus_included
#endinput
#endif
#define _ItemPlus_included

#include <PawnPlus>
#include <item>

#tryinclude <logger>

#include <YSI_Coding\y_hooks>

// In theory we shouldn't need to use any fixed numbers thanks to PawnPlus but getting data from MySQL might be a bit tricky

#if !defined ITEMPLUS_MAX_KEY_LENGTH
    const ITEMPLUS_MAX_KEY_LENGTH = 32;
#endif

#if !defined ITEMPLUS_MAX_ATTRIBUTES
    const ITEMPLUS_MAX_ATTRIBUTES = 10;
#endif

static Map:s_ItemExtraData[MAX_ITEM];

hook OnItemDestroy(Item:itemid) {
    if (map_valid(s_ItemExtraData[itemid])) {
        map_delete_deep(s_ItemExtraData[itemid]);
    }

    return 1;
}

bool:ItemPlus_HasAnyExtraData(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    return map_valid(s_ItemExtraData[itemid]);
}

bool:ItemPlus_HasExtraData(Item:itemid, const key[]) {
    if (!ItemPlus_HasAnyExtraData(itemid)) {
        return false;
    }

    return map_has_str_key(s_ItemExtraData[itemid], key);
}

stock bool:ItemPlus_SetExtraData(Item:itemid, const key[], value) {
    if (strlen(key) > ITEMPLUS_MAX_KEY_LENGTH) {
        #if defined _logger_included
        Logger_Err(
            "[ItemPlus] too long key length passed",
            Logger_I("limit", ITEMPLUS_MAX_KEY_LENGTH)
        );
        #endif
        return false;
    }

    if (!IsValidItem(itemid)) {
        #if defined _logger_included
        Logger_Err(
            "[ItemPlus] attempted to set extra data for an invalid item",
            Logger_I("itemid", _:itemid)
        );
        #endif
        return false;
    }

    if (!map_valid(s_ItemExtraData[itemid])) {
        s_ItemExtraData[itemid] = map_new();
    }

    if (map_size(s_ItemExtraData[itemid]) >= ITEMPLUS_MAX_ATTRIBUTES) {
        #if defined _logger_included
        Logger_Err(
            "[ItemPlus] too many attributes set for an item",
            Logger_I("itemid", _:itemid),
            Logger_I("limit", ITEMPLUS_MAX_ATTRIBUTES)
        );
        #endif
        return false;
    }

    map_str_set(s_ItemExtraData[itemid], key, value);

    return true;
}

stock bool:ItemPlus_GetExtraData(Item:itemid, const key[], &value) {
    if (!ItemPlus_HasExtraData(itemid, key)) {
        return false;
    }

    value = map_str_get(s_ItemExtraData[itemid], key);

    return true;
}

stock bool:ItemPlus_GetExtraDataMap(Item:itemid, &Map:map) {
    if (!ItemPlus_HasAnyExtraData(itemid)) {
        return false;
    }

    map = s_ItemExtraData[itemid];

    return true;
}
