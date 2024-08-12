#include "../redismodule.h"
// #include "../rmutil/util.h"
// #include "../rmutil/strings.h"
// #include "../rmutil/test_util.h"

// #include "redismodule.h"
#include <string.h>
#include <stdio.h>

int LogSet_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 3) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_WRITE);
  size_t val_len;
  const char *val = RedisModule_StringPtrLen(argv[2], &val_len);
  // Perform the SET operation
  RedisModule_StringSet(key, argv[2]);
  // Check if value size > 4 bytes and log if so
  if (val_len > 4) {
    const char *key_str = RedisModule_StringPtrLen(argv[1], NULL);
    RedisModule_Log(ctx, "warning", "LOGSET: Key '%s' set with value of size %zu bytes", key_str,
                    val_len);
  }
  RedisModule_CloseKey(key);
  RedisModule_ReplyWithSimpleString(ctx, "OK");
  return REDISMODULE_OK;
}
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (RedisModule_Init(ctx, "LogSetModule", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }
  if (RedisModule_CreateCommand(ctx, "logset", LogSet_RedisCommand, "write", 1, 1, 1) ==
      REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }
  return REDISMODULE_OK;
}
