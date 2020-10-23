
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: TODO 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>*L;
	L = data.maplayer.start;
	while (L != NULL) // Iterate all layers
	{
		for (int j = 0; j < L->data->height; ++j) // Start at first row
		{
			for (int i = 0; i < L->data->width; ++i) // Iterate all collumns
			{
				uint u= L->data->Get(i, j);
				LOG("%u", u);
				SDL_Rect n = data.tilesets.start->data->GetTileRect(u);
				iPoint pos = MapToWorld(i, j);
				app->render->DrawTexture(data.tilesets.start->data->texture, pos.x, pos.y, &n);
			}
		}

		L = L->next;
	}
	
	// L04: TODO 9: Complete the draw function

}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;
	if (MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (MapTypes::MAPTYPE_ISOMETRIC)
	{
		
	}

	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	
	// L04: TODO 7: Get relative Tile rectangle
	iPoint p = { 0,this->margin };
	int targetId = firstgid;
	for (int j = 0; j < this->numTilesHeight; ++j)
	{
		p.x = this->spacing;
		for (int i = 0; i < this->numTilesWidth; ++i)
		{
			if (id == targetId)
				return SDL_Rect({ p.x,p.y,this->tile_width,tile_height });
			p.x += this->tile_width + this->spacing;
			++targetId;
		}
		p.y += this->tile_height + this->spacing;
	}
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* layerList;
	layerList = data.maplayer.start;

	while (layerList != NULL)
	{
		RELEASE(layerList->data);
		layerList = layerList->next;
	}
	data.maplayer.clear();

	// Clean up the pugui tree
	mapFile.reset();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet(); // Create new tileset

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set); // Add the filled tileset to the list of tilesets
	}

	// L04: TODO 4: Iterate all layers and load each of them
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer(); // Create new tileset

		if (ret == true) ret = LoadLayer(layer, set);

		data.maplayer.add(set); // Add the filled tileset to the list of tilesets
	}

    if(ret == true)
    {
		LogInfo();
    }

    mapLoaded = ret;

    return ret;
}

// L03: TODO: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		LOG("Filling Map info");
		//data.version = map.attribute("version").as_string();
		SString strType(map.attribute("orientation").as_string());
		data.type = StrToMapType(strType);
		//data.renderorder = map.attribute("renderorder").as_string();
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		//data.nextObjectId = map.attribute("nextobjectid").as_int();
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	
	// L03: TODO: Load Tileset attributes
	LOG("Filling TilesetDetails");
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->name = tileset_node.attribute("name").as_string();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->margin = tileset_node.attribute("margin").as_int();

	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		LOG("Filling TilesetDetails");
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tile_width;
		set->numTilesHeight = set->texHeight / set->tile_height;
		set->offsetX = 0;
		set->offsetY = 0;
	}
	return ret;
}

bool Map::StoreId(pugi::xml_node& node, MapLayer* layer, int index)
{
	bool ret = true;

	layer->data[index] = node.attribute("gid").as_uint();

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	// Node points to layer

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->data = new uint[(data.width * data.height * sizeof(uint))];
	memset(layer->data, 0, size_t(data.width * data.height * sizeof(uint)));
	pugi::xml_node gidNode;

	int i = 0;
	for (gidNode = node.child("data").child("tile"); gidNode && ret; gidNode = gidNode.next_sibling("tile"))
	{
		if (ret == true) ret = StoreId(gidNode, layer, i);
		++i;
	}

	LOG("Layer <<%s>> has loaded %d tiles", layer->name.GetString(), i);
	return ret;
}
MapTypes operator++(MapTypes& mode)
{
	mode = static_cast<MapTypes>((mode + 1) % 4);
	return mode;
}
MapTypes Map::StrToMapType(SString s)
{
	SString StrType[4];
	StrType[0]="unknown", StrType[1]="orthogonal", StrType[2]="isometric", StrType[3]="staggered";
	MapTypes type = MapTypes::MAPTYPE_UNKNOWN;
	for (int i = 0; i < 4; ++i)
	{
		if (s == StrType[i])
			return ++type;
	}
	return MAPTYPE_UNKNOWN;
}

void Map::LogInfo()
{
	// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
	LOG("--------------------------------------------------------------------------");
	LOG("<< MAP DATA >>");
	LOG("Width=%d", data.width);
	LOG("Height=%d", data.height);
	LOG("TileWidth=%d", data.tileWidth);
	LOG("TileHeight=%d", data.tileHeight);
	LOG("<< END DATA >>\n");

	ListItem<TileSet*>* infoList;
	infoList = data.tilesets.start;
	while (infoList != NULL)
	{
		LOG("<< TILESET >>");
		LOG("Name=%s", infoList->data->name.GetString());
		LOG("Firstgid=%d", infoList->data->firstgid);
		LOG("Margin=%d", infoList->data->margin);
		LOG("Spacing=%d", infoList->data->spacing);
		LOG("Tile_width=%d", infoList->data->tile_width);
		LOG("Tile_height=%d", infoList->data->tile_height);

		LOG("texWidth=%d", infoList->data->texWidth);
		LOG("texHeight=%d", infoList->data->texHeight);
		LOG("numTilesWidth=%d", infoList->data->numTilesWidth);
		LOG("numTilesHeight=%d", infoList->data->numTilesHeight);
		LOG("<< END TILESET >>\n");

		infoList = infoList->next;
	}

	// L04: TODO 4: LOG the info for each loaded layer
	ListItem<MapLayer*>* layerList;
	layerList = data.maplayer.start;
	while (layerList != NULL)
	{
		LOG("<< LAYER >>");
		LOG("Name=%s", layerList->data->name.GetString());
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);
		LOG("<< END LAYER >>\n");
		layerList = layerList->next;
	}
	LOG("--------------------------------------------------------------------------");
}