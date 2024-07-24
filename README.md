# InventoryExporter

A LeviLamina mod to export all player's inventory and player data to NBT files.

## Usage

1. Install this plugin
2. Start BDS
3. All player's PlayerData / Inventory / EnderChest of current save will be exported to `plugins\InventoryExporter\saved` as Binary NBT files. 
4. Filenames are in the format of `<uuid>-<type>.nbt`

## Tips

- You can create a UUID-Name map file at `plugins\InventoryExporter\uuid-name_map.txt` with contents like below:

  ```
  7ffb1266-3880-355a-bc65-aee375402f1d steve
  e4d83485-895a-350c-bbf2-bb32cecb135d alex
  0dc2d4c4-b8c5-3276-bbd9-e3a125e8c8a2 john
  ca7ebc16-c09c-3b03-b7bf-a3dbc0aa6efb jack
  ...
  ```

- The plugin will replace UUIDs in filename with more readable player names if matched in this file.
