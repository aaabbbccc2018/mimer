#ifndef SHA1_H
#define SHA1_H
#include "../../platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

class sha1
{
/*
 * Helper functions.
 */
#define ROTATE(bits, word)  (((word) << (bits)) | ((word) >> (32 - (bits))))

#define F1(b, c, d)  (((b) & (c)) | ((~(b)) & (d)))
#define F2(b, c, d)  ((b) ^ (c) ^ (d))
#define F3(b, c, d)  (((b) & (c)) | ((b) & (d)) | ((c) & (d)))

#undef  STEP
#define STEP(f, a, b, c, d, e, w, t)                                          \
    temp = ROTATE(5, (a)) + f((b), (c), (d)) + (e) + (w) + (t);               \
    (e) = (d);                                                                \
    (d) = (c);                                                                \
    (c) = ROTATE(30, (b));                                                    \
    (b) = (a);                                                                \
    (a) = temp;


/*
 * GET() reads 4 input bytes in big-endian byte order and returns
 * them as uint32_t.
 */
#undef  GET
#define GET(n)                                                                \
    ((uint32_t) p[n * 4 + 3] |                                                \
    ((uint32_t) p[n * 4 + 2] << 8) |                                          \
    ((uint32_t) p[n * 4 + 1] << 16) |                                         \
    ((uint32_t) p[n * 4] << 24))


    typedef struct {
        uint64_t  bytes;
        uint32_t  a, b, c, d, e, f;
        u_char    buffer[64];
    } sha1_t;
public:
    sha1(){
        _ctx = (sha1_t*)malloc(sizeof(sha1_t));
        memset(_ctx,0,sizeof(sha1_t));
        _ctx->a = 0x67452301;
        _ctx->b = 0xefcdab89;
        _ctx->c = 0x98badcfe;
        _ctx->d = 0x10325476;
        _ctx->e = 0xc3d2e1f0;
        _ctx->bytes = 0;
    }
    ~sha1(){
        if(_ctx){
            free(_ctx);
            _ctx = NULL;
        }
    }
    void sha1_update(const void *data, size_t size){
        _sha1_update(_ctx,data,size);
    }
    std::string sha1_final(){
        u_char result[20] = {0};
        _sha1_final(result,_ctx);
        std::string ret = "";
        char ret_tmp[3] = {0};
        for(int j = 0; j < 20; j++){
            sprintf(ret_tmp, "%02x", result[j]);
            ret += ret_tmp;
        }
        return ret;
    }
private:
    void _sha1_update(sha1_t *ctx, const void *data, size_t size);
    void _sha1_final(u_char result[20], sha1_t *ctx);
    static const u_char * sha1_body(sha1_t *ctx, const u_char *data, size_t size);
private:
    sha1_t* _ctx;
};

#endif // SHA1_H
