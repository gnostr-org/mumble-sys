#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum Mumble_PluginFeature {
	MUMBLE_FEATURE_NONE = 0,
	MUMBLE_FEATURE_POSITIONAL = 1 << 0,
	MUMBLE_FEATURE_AUDIO = 1 << 1,
};

enum Mumble_TalkingState {
	MUMBLE_TS_INVALID = -1,
	MUMBLE_TS_PASSIVE = 0,
	MUMBLE_TS_TALKING,
	MUMBLE_TS_WHISPERING,
	MUMBLE_TS_SHOUTING,
	MUMBLE_TS_TALKING_MUTED,
};

enum Mumble_TransmissionMode {
	MUMBLE_TM_CONTINOUS,
	MUMBLE_TM_VOICE_ACTIVATION,
	MUMBLE_TM_PUSH_TO_TALK,
};

enum Mumble_ErrorCode {
	MUMBLE_EC_INTERNAL_ERROR = -2,
	MUMBLE_EC_GENERIC_ERROR  = -1,
	MUMBLE_EC_OK             = 0,
	MUMBLE_EC_POINTER_NOT_FOUND,
	MUMBLE_EC_NO_ACTIVE_CONNECTION,
	MUMBLE_EC_USER_NOT_FOUND,
	MUMBLE_EC_CHANNEL_NOT_FOUND,
	MUMBLE_EC_CONNECTION_NOT_FOUND,
	MUMBLE_EC_UNKNOWN_TRANSMISSION_MODE,
	MUMBLE_EC_AUDIO_NOT_AVAILABLE,
	MUMBLE_EC_INVALID_SAMPLE,
	MUMBLE_EC_INVALID_PLUGIN_ID,
	MUMBLE_EC_INVALID_MUTE_TARGET,
	MUMBLE_EC_CONNECTION_UNSYNCHRONIZED,
	MUMBLE_EC_INVALID_API_VERSION,
	MUMBLE_EC_UNSYNCHRONIZED_BLOB,
	MUMBLE_EC_UNKNOWN_SETTINGS_KEY,
	MUMBLE_EC_WRONG_SETTINGS_TYPE,
	MUMBLE_EC_SETTING_WAS_REMOVED,
	MUMBLE_EC_DATA_TOO_BIG,
	MUMBLE_EC_DATA_ID_TOO_LONG,
	MUMBLE_EC_API_REQUEST_TIMEOUT,
	MUMBLE_EC_OPERATION_UNSUPPORTED_BY_SERVER,
};

enum Mumble_PositionalDataErrorCode {
	MUMBLE_PDEC_OK = 0,
	MUMBLE_PDEC_ERROR_TEMP,
	MUMBLE_PDEC_ERROR_PERM,
};

enum Mumble_SettingsKey {
	MUMBLE_SK_INVALID                           = -1,
	MUMBLE_SK_AUDIO_INPUT_VOICE_HOLD            = 0,
	MUMBLE_SK_AUDIO_INPUT_VAD_SILENCE_THRESHOLD = 1,
	MUMBLE_SK_AUDIO_INPUT_VAD_SPEECH_THRESHOLD  = 2,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_DISTANCE  = 3,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MAXIMUM_DISTANCE  = 4,
	MUMBLE_SK_AUDIO_OUTPUT_PA_BLOOM             = 5,
	MUMBLE_SK_AUDIO_OUTPUT_PA_MINIMUM_VOLUME    = 6,
};

enum Mumble_KeyCode {
	MUMBLE_KC_INVALID = -1,
	MUMBLE_KC_NULL       = 0,
	MUMBLE_KC_END        = 1,
	MUMBLE_KC_LEFT       = 2,
	MUMBLE_KC_RIGHT      = 4,
	MUMBLE_KC_UP         = 5,
	MUMBLE_KC_DOWN       = 6,
	MUMBLE_KC_DELETE     = 7,
	MUMBLE_KC_BACKSPACE  = 8,
	MUMBLE_KC_TAB        = 9,
	MUMBLE_KC_ENTER      = 10,
	MUMBLE_KC_ESCAPE     = 27,
	MUMBLE_KC_PAGE_UP    = 11,
	MUMBLE_KC_PAGE_DOWN  = 12,
	MUMBLE_KC_SHIFT      = 13,
	MUMBLE_KC_CONTROL    = 14,
	MUMBLE_KC_META       = 15,
	MUMBLE_KC_ALT        = 16,
	MUMBLE_KC_ALT_GR     = 17,
	MUMBLE_KC_CAPSLOCK   = 18,
	MUMBLE_KC_NUMLOCK    = 19,
	MUMBLE_KC_SUPER      = 20,
	MUMBLE_KC_HOME       = 21,
	MUMBLE_KC_PRINT      = 22,
	MUMBLE_KC_SCROLLLOCK = 23,
	MUMBLE_KC_SPACE             = ' ',
	MUMBLE_KC_EXCLAMATION_MARK  = '!',
	MUMBLE_KC_DOUBLE_QUOTE      = '"',
	MUMBLE_KC_HASHTAG           = '#',
	MUMBLE_KC_DOLLAR            = '$',
	MUMBLE_KC_PERCENT           = '%',
	MUMBLE_KC_AMPERSAND         = '&',
	MUMBLE_KC_SINGLE_QUOTE      = "'",
	MUMBLE_KC_OPEN_PARENTHESIS  = '(' ,
	MUMBLE_KC_CLOSE_PARENTHESIS = ')' ,
	MUMBLE_KC_ASTERISK          = '*' ,
	MUMBLE_KC_PLUS              = '+' ,
	MUMBLE_KC_COMMA             = ',' ,
	MUMBLE_KC_MINUS             = '-' ,
	MUMBLE_KC_PERIOD            = '.' ,
	MUMBLE_KC_SLASH             = '/' ,
	MUMBLE_KC_0                 = '0' ,
	MUMBLE_KC_1                 = '1' ,
	MUMBLE_KC_2                 = '2' ,
	MUMBLE_KC_3                 = '3' ,
	MUMBLE_KC_4                 = '4' ,
	MUMBLE_KC_5                 = '5' ,
	MUMBLE_KC_6                 = '6' ,
	MUMBLE_KC_7                 = '7' ,
	MUMBLE_KC_8                 = '8' ,
	MUMBLE_KC_9                 = '9' ,
	MUMBLE_KC_COLON             = ':' ,
	MUMBLE_KC_SEMICOLON         = ';' ,
	MUMBLE_KC_LESS_THAN         = '<' ,
	MUMBLE_KC_EQUALS            = '=' ,
	MUMBLE_KC_GREATER_THAN      = '>' ,
	MUMBLE_KC_QUESTION_MARK     = '?' ,
	MUMBLE_KC_AT_SYMBOL         = '@' ,
	MUMBLE_KC_A                 = 'A' ,
	MUMBLE_KC_B                 = 'B' ,
	MUMBLE_KC_C                 = 'C' ,
	MUMBLE_KC_D                 = 'D' ,
	MUMBLE_KC_E                 = 'E' ,
	MUMBLE_KC_F                 = 'F' ,
	MUMBLE_KC_G                 = 'G' ,
	MUMBLE_KC_H                 = 'H' ,
	MUMBLE_KC_I                 = 'I' ,
	MUMBLE_KC_J                 = 'J' ,
	MUMBLE_KC_K                 = 'K' ,
	MUMBLE_KC_L                 = 'L' ,
	MUMBLE_KC_M                 = 'M' ,
	MUMBLE_KC_N                 = 'N' ,
	MUMBLE_KC_O                 = 'O' ,
	MUMBLE_KC_P                 = 'P' ,
	MUMBLE_KC_Q                 = 'Q' ,
	MUMBLE_KC_R                 = 'R' ,
	MUMBLE_KC_S                 = 'S' ,
	MUMBLE_KC_T                 = 'T' ,
	MUMBLE_KC_U                 = 'U' ,
	MUMBLE_KC_V                 = 'V' ,
	MUMBLE_KC_W                 = 'W' ,
	MUMBLE_KC_X                 = 'X' ,
	MUMBLE_KC_Y                 = 'Y' ,
	MUMBLE_KC_Z                 = 'Z' ,
	MUMBLE_KC_OPEN_BRACKET  = '[' ,
	MUMBLE_KC_BACKSLASH     = '\' ,
	MUMBLE_KC_CLOSE_BRACKET = ']' ,
	MUMBLE_KC_CIRCUMFLEX    = '^' ,
	MUMBLE_KC_UNDERSCORE    = '_' ,
	MUMBLE_KC_GRAVE_AKCENT  = '`' ,
	MUMBLE_KC_OPEN_BRACE    = '{' ,
	MUMBLE_KC_VERTICAL_BAR  = '|' ,
	MUMBLE_KC_CLOSE_BRACE   = '}' ,
	MUMBLE_KC_TILDE         = '~' ,
	MUMBLE_KC_DEGREE_SIGN = 176,
	MUMBLE_KC_F1  = 256,
	MUMBLE_KC_F2  = 257,
	MUMBLE_KC_F3  = 258,
	MUMBLE_KC_F4  = 259,
	MUMBLE_KC_F5  = 260,
	MUMBLE_KC_F6  = 261,
	MUMBLE_KC_F7  = 262,
	MUMBLE_KC_F8  = 263,
	MUMBLE_KC_F9  = 264,
	MUMBLE_KC_F10 = 265,
	MUMBLE_KC_F11 = 266,
	MUMBLE_KC_F12 = 267,
	MUMBLE_KC_F13 = 268,
	MUMBLE_KC_F14 = 269,
	MUMBLE_KC_F15 = 270,
	MUMBLE_KC_F16 = 271,
	MUMBLE_KC_F17 = 272,
	MUMBLE_KC_F18 = 273,
	MUMBLE_KC_F19 = 274,
};

struct MumbleVersion {
	int32_t major;
	int32_t minor;
	int32_t patch;
};

struct MumbleStringWrapper {
	const char *data;
	size_t size;
	bool needsReleasing;
};

typedef enum Mumble_TalkingState mumble_talking_state_t;
typedef enum Mumble_TransmissionMode mumble_transmission_mode_t;
typedef struct MumbleVersion mumble_version_t;
typedef int32_t mumble_connection_t;
typedef uint32_t mumble_userid_t;
typedef int32_t mumble_channelid_t;
typedef enum Mumble_ErrorCode mumble_error_t;
typedef uint32_t mumble_plugin_id_t;
typedef enum Mumble_SettingsKey mumble_settings_key_t;
typedef enum Mumble_KeyCode mumble_keycode_t;

// Define the MumbleAPI_v_1_2_0 struct (assuming API version 1.2.x based on MumblePlugin.h)
// This needs to be manually constructed based on the function pointers in MumblePlugin.h
// I'm using a generic name here, as the exact version might vary.
// For simplicity, I'll use MumbleAPI_v_1_2_0 as a placeholder.
// You would need to adjust this if the actual API version used is different.
struct MumbleAPI_v_1_2_0 {
    mumble_error_t (*freeMemory)(mumble_plugin_id_t callerID, const void *pointer);
    mumble_error_t (*getActiveServerConnection)(mumble_plugin_id_t callerID, mumble_connection_t *connection);
    mumble_error_t (*isConnectionSynchronized)(mumble_plugin_id_t callerID, mumble_connection_t connection, bool *synchronized);
    mumble_error_t (*getLocalUserID)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t *userID);
    mumble_error_t (*getUserName)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **userName);
    mumble_error_t (*getChannelName)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, const char **channelName);
    mumble_error_t (*getAllUsers)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t **users, size_t *userCount);
    mumble_error_t (*getAllChannels)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t **channels, size_t *channelCount);
    mumble_error_t (*getChannelOfUser)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, mumble_channelid_t *channel);
    mumble_error_t (*getUsersInChannel)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, mumble_userid_t **userList, size_t *userCount);
    mumble_error_t (*getLocalUserTransmissionMode)(mumble_plugin_id_t callerID, mumble_transmission_mode_t *transmissionMode);
    mumble_error_t (*isUserLocallyMuted)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, bool *muted);
    mumble_error_t (*isLocalUserMuted)(mumble_plugin_id_t callerID, bool *muted);
    mumble_error_t (*isLocalUserDeafened)(mumble_plugin_id_t callerID, bool *deafened);
    mumble_error_t (*getUserHash)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **hash);
    mumble_error_t (*getServerHash)(mumble_plugin_id_t callerID, mumble_connection_t connection, const char **hash);
    mumble_error_t (*getUserComment)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, const char **comment);
    mumble_error_t (*getChannelDescription)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_channelid_t channelID, const char **description);
    mumble_error_t (*requestLocalUserTransmissionMode)(mumble_plugin_id_t callerID, mumble_transmission_mode_t transmissionMode);
    mumble_error_t (*requestUserMove)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, mumble_channelid_t channelID, const char *password);
    mumble_error_t (*requestMicrophoneActivationOvewrite)(mumble_plugin_id_t callerID, bool activate);
    mumble_error_t (*requestLocalMute)(mumble_plugin_id_t callerID, mumble_connection_t connection, mumble_userid_t userID, bool muted);
    mumble_error_t (*requestLocalUserMute)(mumble_plugin_id_t callerID, bool muted);
    mumble_error_t (*requestLocalUserDeaf)(mumble_plugin_id_t callerID, bool deafened);
    mumble_error_t (*requestSetLocalUserComment)(mumble_plugin_id_t callerID, mumble_connection_t connection, const char *comment);
    mumble_error_t (*findUserByName)(mumble_plugin_id_t callerID, mumble_connection_t connection, const char *userName, mumble_userid_t *userID);
    mumble_error_t (*findChannelByName)(mumble_plugin_id_t callerID, mumble_connection_t connection, const char *channelName, mumble_channelid_t *channelID);
    mumble_error_t (*getMumbleSetting_bool)(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool *outValue);
    mumble_error_t (*getMumbleSetting_int)(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t *outValue);
    mumble_error_t (*getMumbleSetting_double)(mumble_plugin_id_t callerID, mumble_settings_key_t key, double *outValue);
    mumble_error_t (*getMumbleSetting_string)(mumble_plugin_id_t callerID, mumble_settings_key_t key, const char **outValue);
    mumble_error_t (*setMumbleSetting_bool)(mumble_plugin_id_t callerID, mumble_settings_key_t key, bool value);
    mumble_error_t (*setMumbleSetting_int)(mumble_plugin_id_t callerID, mumble_settings_key_t key, int64_t value);
    mumble_error_t (*setMumbleSetting_double)(mumble_plugin_id_t callerID, mumble_settings_key_t key, double value);
    mumble_error_t (*setMumbleSetting_string)(mumble_plugin_id_t callerID, mumble_settings_key_t key, const char *value);
    mumble_error_t (*sendData)(mumble_plugin_id_t callerID, mumble_connection_t connection, const mumble_userid_t *users, size_t userCount, const uint8_t *data, size_t dataLength, const char *dataID);
    mumble_error_t (*log)(mumble_plugin_id_t callerID, const char *message);
    mumble_error_t (*playSample)(mumble_plugin_id_t callerID, const char *samplePath, float volume);
};

#ifdef __cplusplus
}
#endif
