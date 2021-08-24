#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
void Initialize(int format, char* url, int width, int height, int depth);
void SetImage(void* pImage, int size);
void Finalize();

#ifdef __cplusplus
}
#endif

#endif  // WRAPPER_H_
