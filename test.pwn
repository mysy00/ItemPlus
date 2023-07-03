#include <open.mp>
#include "ItemPlus.inc"

main() 
{
  new ItemType:itemType_Flowers = DefineItemType("Flowers", "Flowers", 325, 1);
  new Item:item_Flowers = CreateItem(itemType_Flowers);

  ItemPlus_SetExtraData(item_Flowers, "lifespan", 4);
  
  new lifespan;
  ItemPlus_GetExtraData(item_Flowers, "lifespan", lifespan);
  printf("The lifespan: %d", lifespan);
}
