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

#include "alewrap.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <string.h>

void ale_loadRom(ALEInterface *ale, const char* rom_file, const char* core_file){
	printf("rom_file: %s, coreFile: %s", rom_file, core_file);
	ale->loadROM(std::string(rom_file), std::string(core_file));
}

//void ale_fillRgbFromPalette(uint8_t *rgb, const uint8_t *obs, size_t rgb_size,
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
//    ALEInterface::getRGB(obs[index], r, g, b);
//
//    rgb[r_offset + index] = r;
//    rgb[g_offset + index] = g;
//    rgb[b_offset + index] = b;
//  }
//}


void ale_rearrangeRgb(uint8_t *obs, const uint32_t *screenArray, size_t rgb_size, size_t obs_size , const ALEInterface *ale) {
  assert(rgb_size == 3*obs_size);

  const int r_offset = 0ul;
  const int g_offset = obs_size;
  const int b_offset = 2ul * obs_size;
  uint8_t r, g, b;
  for (int index = 0ul; index < obs_size; ++index) {

//	  ALEInterface::getRGB(((uint16_t*)screenArray)[index], r, g, b);
	ale->getScreen().getRGB(((uint16_t*)screenArray)[index], r, g, b);
//	fprintf(a,"index:%d  screenArray:%x  r:%x  g:%x  b:%x  \n",index, screenArray[index], r, g, b);
    obs[r_offset + index] = r;
    obs[g_offset + index] = g;
    obs[b_offset + index] = b;
  }
}

static void ale_getGray(uint8_t *obs, const uint32_t *screenArray, size_t rgb_size, size_t obs_size) {


//	assert(rgb_size == obs_size);
	  const int r_offset = 0ul;
	  const int g_offset = obs_size;
	  const int b_offset = 2ul * obs_size;
	  uint8_t r, g, b;
	  for (int index = 0ul; index < obs_size; ++index) {
//	    ALEInterface::getRGB(screenArray[index], r, g, b);
	    obs[index] = (r + g + b)/3;
//	    obs[index] = (uint8_t)screenArray[index];
	  }

	  }

//ALEInterface *ale_new(const char *rom_file, const char *core_file) {
//  return new ALEInterface(rom_file, core_file);
//}

ALEInterface *ale_new() {
  return new ALEInterface();
}

void ale_gc(ALEInterface *ale) {
	delete ale;
}

double ale_act(ALEInterface *ale, int actionA, int actionB) {
  return ale->act(static_cast<ale::Action>(actionA), static_cast<ale::Action>(actionB));
}

int ale_getScreenWidth(const ALEInterface *ale) {
  return ale->getScreen().width();
}

int ale_getScreenHeight(const ALEInterface *ale) {
  return ale->getScreen().height();
}

bool ale_isGameOver(const ALEInterface *ale) { return ale->game_over(); }

void ale_resetGame(ALEInterface *ale) {
  ale->reset_game();
  assert(!ale->game_over());
}

void ale_loadState(ALEInterface *ale) { return ale->loadState(); }

void ale_saveState(ALEInterface *ale) { ale->saveState(); }

void ale_fillObs(const ALEInterface *ale, uint8_t *obs, size_t obs_size) {
	const ale::ALEScreen& screen = ale->getScreen();
	size_t h = screen.height();
	size_t w = screen.width();
	assert(obs_size == 3 * h * w);
//	ale_rearrangeRgb(obs, (uint32_t*)screen.getArray(), screen.arraySize(),obs_size);
	ale_rearrangeRgb(obs, (uint32_t*)screen.getArray(), obs_size,screen.arraySize(),ale);
//  std::copy(screen.getArray().begin(), screen.getArray().end(), obs);
//  memcpy(obs, screen.getArray(), screen.arraySize());
}

void ale_fillObsGray(const ALEInterface *ale, uint8_t *obs, size_t obs_size) {
	const ale::ALEScreen& screen = ale->getScreen();
	size_t h = screen.height();
	size_t w = screen.width();
	//assert(obs_size == h * w);
	ale_getGray(obs, (uint32_t*)screen.getArray(),screen.arraySize(),obs_size);
//  size_t Bpp = ale->getScreen().Bpp();
//  ale_getGray(obs, (uint32_t*)screen.getArray(), obs_size, screen.arraySize(),Bpp);
}

void ale_fillRamObs(const ALEInterface *ale, uint8_t *ram,
                    size_t ram_size) {
  const ale::ALERAM& ale_ram = ale->getRAM();
  assert(ram_size == ale_ram.size());

  const unsigned char* ram_content = ale_ram.array();
  std::copy(ram_content, ram_content + ram_size, ram);
}

int ale_numLegalActions(ALEInterface *ale) {
  return static_cast<int>(ale->getMinimalActionSet().size());
}

void ale_legalActions(ALEInterface *ale, int *actions,
                      size_t actions_size) {
  const std::vector<ale::Action>& legal_actions = ale->getMinimalActionSet();
  assert(actions_size == legal_actions.size());
  std::copy(legal_actions.begin(), legal_actions.end(), actions);
}

int ale_livesRemained(const ALEInterface *ale) { return ale->lives(); }


// Get the value of a setting.
const char* ale_getString(ALEInterface *ale, const char* key){
	return ale->getString(std::string(key)).c_str();
}

int ale_getInt(ALEInterface *ale, const char* key){
	return ale->getInt(std::string(key));
}

bool ale_getBool(ALEInterface *ale, const char* key){
	return ale->getBool(std::string(key));
}

float ale_getFloat(ALEInterface *ale, const char* key){
	return ale->getFloat(std::string(key));
}

// Set the value of a setting. loadRom() must be called before the
// setting will take effect.
void ale_setString(ALEInterface *ale, const char* key, const char* value){
	ale->setString(std::string(key), std::string(value));
}

void ale_setInt(ALEInterface *ale, const char* key, const int value){
	ale->setInt(std::string(key), value);
}

void ale_setBool(ALEInterface *ale, const char* key, const bool value){
	ale->setBool(std::string(key), value);
}

void ale_setFloat(ALEInterface *ale, const char* key, const float value){
	ale->setFloat(std::string(key), value);
}

//int ale_getSnapshotLength(const ALEInterface *ale) {
//  return static_cast<int>(ale->getSnapshot().size());
//}
//
//void ale_saveSnapshot(const ALEInterface *ale, uint8_t *data,
//                      size_t length) {
//  std::string result = ale->getSnapshot();
//
//  assert(length >= result.size() && length > 0);
//
//  if (length < result.size())
//    data = NULL;
//  else
//    result.copy(reinterpret_cast<char *>(data), length);
//}
//
//void ale_restoreSnapshot(ALEInterface *ale, const uint8_t *snapshot,
//                         size_t size) {
//  assert(size > 0);
//
//  std::string snapshotStr(reinterpret_cast<char const *>(snapshot), size);
//  ale->restoreSnapshot(snapshotStr);
//}

