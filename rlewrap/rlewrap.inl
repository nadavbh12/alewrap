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

// explicit declare
typedef unsigned char uint8_t;

// Re-arranges the pixels from a consecutive rgb per pixel to three arrays:
// r-array, g-array, b-array.
// The size is the size of the array, not size in bytes 
//void rle_rearrangeRgb(uint8_t *rgb, const uint32_t *obs, size_t rgb_size,
//                            size_t obs_size);

// Initializes the RLE.
//RLEInterface *rle_new(const char *rom_file,  const char *core_file);
RLEInterface *rle_new();

// load a rom and a core to rle
void rle_loadRom(RLEInterface *rle, const char* rom_file, const char* core_file);

// Deletes the RLE pointer.
void rle_gc(RLEInterface *rle);

// Applies the action and returns the obtained reward.
double rle_act(RLEInterface *rle,  int actionA, int actionB);

// Returns the screen width.
int rle_getScreenWidth(const RLEInterface *rle);

// Returns the screen height.
int rle_getScreenHeight(const RLEInterface *rle);

// Indicates whether the game ended.
// Call resetGame to restart the game.
//
// Returning of bool instead of int is important.
// The bool is converted to lua bool by FFI.
bool rle_isGameOver(const RLEInterface *rle);

// Resets the game.
void rle_resetGame(RLEInterface *rle);

// RLE save state
void rle_saveState(RLEInterface *rle);

// RLE load state
void rle_loadState(RLEInterface *rle);

// Fills the obs with raw rgb values.
// rle screen holds the screen where every pixel is represented by 32bits of rgb
void rle_fillObs(const RLEInterface *rle, uint8_t *obs, size_t obs_size);

// Fills the obs with raw gray values.
// Value is obtained by averaging r,g,b.
void rle_fillObsGray(const RLEInterface *rle, uint8_t *obs, size_t obs_size);

// Fills the given array with the content of the RAM.
// The obs_size should be 128.
void rle_fillRamObs(const RLEInterface *rle, uint8_t *obs, size_t obs_size);

// Returns the number of legal actions
int rle_numLegalActions(RLEInterface *rle);

// Returns the valid actions for a game
void rle_legalActions(RLEInterface *rle, int *actions, size_t size);

// Returns the number of remaining lives for a game
int rle_livesRemained(const RLEInterface *rle);

//// Used by api to create a string of correct size.
//int rle_getSnapshotLength(const RLEInterface *rle);
//
//// Save the current state into a snapshot
//void rle_saveSnapshot(const RLEInterface *rle, uint8_t *data, size_t length);
//
//// Load a particular snapshot into the emulator
//void rle_restoreSnapshot(RLEInterface *rle, const uint8_t *snapshot,
//                         size_t size);

// Get the value of a setting.
const char* rle_getString(RLEInterface *rle, const char* key);

int rle_getInt(RLEInterface *rle, const char* key);

bool rle_getBool(RLEInterface *rle, const char* key);

float rle_getFloat(RLEInterface *rle, const char* key);

// Set the value of a setting. loadRom() must be called before the
// setting will take effect.
void rle_setString(RLEInterface *rle, const char* key, const char* value);

void rle_setInt(RLEInterface *rle, const char* key, const int value);

void rle_setBool(RLEInterface *rle, const char* key, const bool value);

void rle_setFloat(RLEInterface *rle, const char* key, const float value);
