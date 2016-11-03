/* Copyright 2014 Google Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "rlewrap.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <string.h>

void rle_loadRom(RLEInterface *rle, const char* rom_file, const char* core_file){
	printf("rom_file: %s, coreFile: %s", rom_file, core_file);
	rle->loadROM(std::string(rom_file), std::string(core_file));
}

//void rle_fillRgbFromPrlette(uint8_t *rgb, const uint8_t *obs, size_t rgb_size,
//                            size_t obs_size) {
//  assert(obs_size >= 0);
//  assert(rgb_size == 3 * obs_size);
//
//  const int r_offset = 0ul;
//  const int g_offset = obs_size;
//  const int b_offset = 2ul * obs_size;
//
//  for (int index = 0ul; index < obs_size; ++index) {
//    uint8_t r, g, b;
//    RLEInterface::getRGB(obs[index], r, g, b);
//
//    rgb[r_offset + index] = r;
//    rgb[g_offset + index] = g;
//    rgb[b_offset + index] = b;
//  }
//}


void rle_rearrangeRgb(uint8_t *obs, const uint32_t *screenArray, size_t rgb_size, size_t obs_size , const RLEInterface *rle) {
  assert(rgb_size == 3*obs_size);

  const int r_offset = 0ul;
  const int g_offset = obs_size;
  const int b_offset = 2ul * obs_size;
  uint8_t r, g, b;
  for (int index = 0ul; index < obs_size; ++index) {

//	  RLEInterface::getRGB(((uint16_t*)screenArray)[index], r, g, b);
	rle->getScreen().getRGB(((uint16_t*)screenArray)[index], r, g, b);
//	fprintf(a,"index:%d  screenArray:%x  r:%x  g:%x  b:%x  \n",index, screenArray[index], r, g, b);
    obs[r_offset + index] = r;
    obs[g_offset + index] = g;
    obs[b_offset + index] = b;
  }
}

static void rle_getGray(uint8_t *obs, const uint32_t *screenArray, size_t rgb_size, size_t obs_size) {


//	assert(rgb_size == obs_size);
	  const int r_offset = 0ul;
	  const int g_offset = obs_size;
	  const int b_offset = 2ul * obs_size;
	  uint8_t r, g, b;
	  for (int index = 0ul; index < obs_size; ++index) {
//	    RLEInterface::getRGB(screenArray[index], r, g, b);
	    obs[index] = (r + g + b)/3;
//	    obs[index] = (uint8_t)screenArray[index];
	  }

	  }

//RLEInterface *rle_new(const char *rom_file, const char *core_file) {
//  return new RLEInterface(rom_file, core_file);
//}

RLEInterface *rle_new() {
  return new RLEInterface();
}

void rle_gc(RLEInterface *rle) {
	delete rle;
}

double rle_act(RLEInterface *rle, int actionA, int actionB) {
  return rle->act(static_cast<rle::Action>(actionA), static_cast<rle::Action>(actionB));
}

int rle_getScreenWidth(const RLEInterface *rle) {
  return rle->getScreen().width();
}

int rle_getScreenHeight(const RLEInterface *rle) {
  return rle->getScreen().height();
}

bool rle_isGameOver(const RLEInterface *rle) { return rle->game_over(); }

void rle_resetGame(RLEInterface *rle) {
  rle->reset_game();
  assert(!rle->game_over());
}

void rle_loadState(RLEInterface *rle) { return rle->loadState(); }

void rle_saveState(RLEInterface *rle) { rle->saveState(); }

void rle_fillObs(const RLEInterface *rle, uint8_t *obs, size_t obs_size) {
	const rle::RLEScreen& screen = rle->getScreen();
	size_t h = screen.height();
	size_t w = screen.width();
	assert(obs_size == 3 * h * w);
//	rle_rearrangeRgb(obs, (uint32_t*)screen.getArray(), screen.arraySize(),obs_size);
	rle_rearrangeRgb(obs, (uint32_t*)screen.getArray(), obs_size,screen.arraySize(),rle);
//  std::copy(screen.getArray().begin(), screen.getArray().end(), obs);
//  memcpy(obs, screen.getArray(), screen.arraySize());
}

void rle_fillObsGray(const RLEInterface *rle, uint8_t *obs, size_t obs_size) {
	const rle::RLEScreen& screen = rle->getScreen();
	size_t h = screen.height();
	size_t w = screen.width();
	//assert(obs_size == h * w);
	rle_getGray(obs, (uint32_t*)screen.getArray(),screen.arraySize(),obs_size);
//  size_t Bpp = rle->getScreen().Bpp();
//  rle_getGray(obs, (uint32_t*)screen.getArray(), obs_size, screen.arraySize(),Bpp);
}

void rle_fillRamObs(const RLEInterface *rle, uint8_t *ram,
                    size_t ram_size) {
  const rle::RLERAM& rle_ram = rle->getRAM();
  assert(ram_size == rle_ram.size());

  const unsigned char* ram_content = rle_ram.array();
  std::copy(ram_content, ram_content + ram_size, ram);
}

int rle_numLegalActions(RLEInterface *rle) {
  return static_cast<int>(rle->getMinimalActionSet().size());
}

void rle_legalActions(RLEInterface *rle, int *actions,
                      size_t actions_size) {
  const std::vector<rle::Action>& legal_actions = rle->getMinimalActionSet();
  assert(actions_size == legal_actions.size());
  std::copy(legal_actions.begin(), legal_actions.end(), actions);
}

int rle_livesRemained(const RLEInterface *rle) { return rle->lives(); }


// Get the value of a setting.
const char* rle_getString(RLEInterface *rle, const char* key){
	return rle->getString(std::string(key)).c_str();
}

int rle_getInt(RLEInterface *rle, const char* key){
	return rle->getInt(std::string(key));
}

bool rle_getBool(RLEInterface *rle, const char* key){
	return rle->getBool(std::string(key));
}

float rle_getFloat(RLEInterface *rle, const char* key){
	return rle->getFloat(std::string(key));
}

// Set the value of a setting. loadRom() must be called before the
// setting will take effect.
void rle_setString(RLEInterface *rle, const char* key, const char* value){
	rle->setString(std::string(key), std::string(value));
}

void rle_setInt(RLEInterface *rle, const char* key, const int value){
	rle->setInt(std::string(key), value);
}

void rle_setBool(RLEInterface *rle, const char* key, const bool value){
	rle->setBool(std::string(key), value);
}

void rle_setFloat(RLEInterface *rle, const char* key, const float value){
	rle->setFloat(std::string(key), value);
}

//int rle_getSnapshotLength(const RLEInterface *rle) {
//  return static_cast<int>(rle->getSnapshot().size());
//}
//
//void rle_saveSnapshot(const RLEInterface *rle, uint8_t *data,
//                      size_t length) {
//  std::string result = rle->getSnapshot();
//
//  assert(length >= result.size() && length > 0);
//
//  if (length < result.size())
//    data = NULL;
//  else
//    result.copy(reinterpret_cast<char *>(data), length);
//}
//
//void rle_restoreSnapshot(RLEInterface *rle, const uint8_t *snapshot,
//                         size_t size) {
//  assert(size > 0);
//
//  std::string snapshotStr(reinterpret_cast<char const *>(snapshot), size);
//  rle->restoreSnapshot(snapshotStr);
//}

