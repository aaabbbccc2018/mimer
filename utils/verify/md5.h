#ifndef MD5_H
#define MD5_H
#include "../../platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

class md5
{

#define NGX_HAVE_LITTLE_ENDIAN  0
#define NGX_HAVE_NONALIGNED     0

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#define F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)  ((x) ^ (y) ^ (z))
#define I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#define STEP(f, a, b, c, d, x, t, s)                                          \
    (a) += f((b), (c), (d)) + (x) + (t);                                      \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));                \
    (a) += (b)

/*
 * SET() reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * does not work.
 */

#if (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#define SET(n)      (*(uint32_t *) &p[n * 4])
#define GET(n)      (*(uint32_t *) &p[n * 4])

#else

#define SET(n)                                                                \
    (block[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#define GET(n)      block[n]

#endif

    typedef struct{
        uint64_t  bytes;
        uint32_t  a, b, c, d;
        u_char    buffer[64];
    }md5_t;
public:
    md5():_ctx(NULL){
        _ctx = (md5_t*)malloc(sizeof(md5_t));
        memset(_ctx,0,sizeof(md5_t));
        _ctx->a = 0x67452301;
        _ctx->b = 0xefcdab89;
        _ctx->c = 0x98badcfe;
        _ctx->d = 0x10325476;
        _ctx->bytes = 0;
    }
    ~md5(){
        if(_ctx){
            free(_ctx);
            _ctx = NULL;
        }
    }
public:
    void md5_update(const void *data, size_t size){
        _md5_update(_ctx,data,size);
    }
    std::string md5_final(){
        u_char result[16] = {};
        _md5_final(result,_ctx);
        std::string ret = "";
        char ret_tmp[3] = {0};
        for(int j = 0; j < 16; j++){
            sprintf(ret_tmp, "%02x", result[j]);
            ret += ret_tmp;
        }
        return ret;
    }
private:
    void _md5_update(md5_t *ctx, const void *data, size_t size);
    void _md5_final(u_char result[16], md5_t *ctx);
    static const u_char * md5_body(md5_t *ctx, const u_char *data, size_t size);
private:
    md5_t*      _ctx;
};

#endif // MD5_H
