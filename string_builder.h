// string_builder.h by Mzying2001

#ifndef __STRING_BUILDER_H__
#define __STRING_BUILDER_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _STRING_BUILDER_HERE_SIZE (16)

typedef enum
{
    _SBIT_HERE,
    _SBIT_THERE
} _STRING_BUILDER_ITEM_TYPE;

typedef struct _string_builder_item_struct
{
    _STRING_BUILDER_ITEM_TYPE _type;
    size_t _len;
    union
    {
        char _here[_STRING_BUILDER_HERE_SIZE];
        char *_there;
    } _val;
    struct _string_builder_item_struct *_next;
} _string_builder_item;

int _sbi_init(_string_builder_item *pitem, const char *str)
{
    static const char EMPTY_STR[] = "";
    if (pitem == NULL)
    {
        return 0;
    }
    if (str == NULL)
    {
        str = EMPTY_STR;
    }
    const char *p = str;
    size_t len = strlen(str);
    if (len < _STRING_BUILDER_HERE_SIZE)
    {
        strcpy(pitem->_val._here, str);
        pitem->_type = _SBIT_HERE;
    }
    else
    {
        char *there = (char *)calloc(len + 1, sizeof(char));
        if (there == NULL)
        {
            return 0;
        }
        strcpy(there, str);
        pitem->_val._there = there;
        pitem->_type = _SBIT_THERE;
    }
    pitem->_len = len;
    pitem->_next = NULL;
    return 1;
}

_string_builder_item *_sbi_new(const char *str)
{
    _string_builder_item *ret = (_string_builder_item *)malloc(sizeof(_string_builder_item));
    if (ret != NULL)
    {
        if (!_sbi_init(ret, str))
        {
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

void _sbi_free(_string_builder_item *pitem)
{
    if (pitem == NULL)
    {
        return;
    }
    if (pitem->_type == _SBIT_THERE)
    {
        free(pitem->_val._there);
        pitem->_val._there = NULL;
    }
    free(pitem);
}

_string_builder_item *_sbi_clear(_string_builder_item *pitem)
{
    if (pitem == NULL)
    {
        return NULL;
    }
    if (pitem->_type == _SBIT_THERE)
    {
        free(pitem->_val._there);
        pitem->_val._there = NULL;
        pitem->_len = 0;
        pitem->_type = _SBIT_HERE;
    }
    else
    {
        pitem->_len = 0;
        pitem->_val._here[0] = '\0';
    }
    return pitem;
}

inline char *_sbi_getval(_string_builder_item *pitem_not_null)
{
    return pitem_not_null->_type == _SBIT_HERE
               ? pitem_not_null->_val._here
               : pitem_not_null->_val._there;
}

typedef struct
{
    _string_builder_item _root;
    _string_builder_item *_tail;
} string_builder;

int sb_init(string_builder *psb)
{
    if (psb == NULL)
    {
        return 0;
    }
    psb->_tail = &psb->_root;
    return _sbi_init(&psb->_root, "");
}

string_builder *sb_clear(string_builder *psb)
{
    if (psb == NULL)
    {
        return NULL;
    }
    _string_builder_item *p = psb->_root._next;
    while (p)
    {
        _string_builder_item *next = p->_next;
        _sbi_free(p);
        p = next;
    }
    psb->_root._next = NULL;
    psb->_tail = &psb->_root;
    _sbi_clear(&psb->_root);
    return psb;
}

string_builder *sb_append(string_builder *psb, const char *str)
{
    if (psb == NULL)
    {
        return NULL;
    }
    if (str == NULL)
    {
        return psb;
    }
    size_t len = strlen(str);
    _string_builder_item *pitem = psb->_tail;
    if (pitem->_len == 0)
    {
        if (!_sbi_init(pitem, str))
        {
            return NULL;
        }
    }
    else if (pitem->_type == _SBIT_HERE && len + pitem->_len < _STRING_BUILDER_HERE_SIZE)
    {
        strcpy(pitem->_val._here + pitem->_len, str);
        pitem->_len += len;
    }
    else
    {
        pitem->_next = _sbi_new(str);
        if (pitem->_next == NULL)
        {
            return NULL;
        }
        else
        {
            psb->_tail = pitem->_next;
        }
    }
    return psb;
}

string_builder *sb_appendc(string_builder *psb, char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    return sb_append(psb, buf);
}

string_builder *sb_appendi(string_builder *psb, int val)
{
    char buf[16];
    sprintf(buf, "%d", val);
    return sb_append(psb, buf);
}

string_builder *sb_appendf(string_builder *psb, double val)
{
    char buf[32];
    if (val > 1e16)
    {
        sprintf(buf, "%e", val);
    }
    else
    {
        sprintf(buf, "%f", val);
    }
    return sb_append(psb, buf);
}

string_builder *sb_insert(string_builder *psb, size_t index, const char *str)
{
    if (psb == NULL)
    {
        return NULL;
    }
    if (str == NULL)
    {
        return psb;
    }
    size_t i = 0;
    _string_builder_item *pitem = &psb->_root;
    while (pitem && i + pitem->_len <= index)
    {
        i += pitem->_len;
        pitem = pitem->_next;
    }
    if (pitem == NULL)
    {
        return sb_append(psb, str);
    }
    index -= i;
    _string_builder_item *insert_item = _sbi_new(NULL);
    if (insert_item == NULL)
    {
        return NULL;
    }
    if (index == 0)
    {
        *insert_item = *pitem;
        if (_sbi_init(pitem, str))
        {
            pitem->_next = insert_item;
            if (insert_item->_next == NULL)
            {
                psb->_tail = insert_item;
            }
            return psb;
        }
        else
        {
            free(insert_item);
            return NULL;
        }
    }
    if (!_sbi_init(insert_item, str))
    {
        free(insert_item);
        return NULL;
    }
    _string_builder_item *insert_item2 = _sbi_new(_sbi_getval(pitem) + index);
    if (insert_item2 == NULL)
    {
        free(insert_item);
        return NULL;
    }
    _sbi_getval(pitem)[index] = '\0';
    insert_item2->_next = pitem->_next;
    insert_item->_next = insert_item2;
    pitem->_next = insert_item;
    pitem->_len = index;
    if (insert_item2->_next == NULL)
    {
        psb->_tail = insert_item2;
    }
    return psb;
}

string_builder *sb_insertc(string_builder *psb, size_t index, char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    return sb_insert(psb, index, buf);
}

string_builder *sb_inserti(string_builder *psb, size_t index, int val)
{
    char buf[16];
    sprintf(buf, "%d", val);
    return sb_insert(psb, index, buf);
}

string_builder *sb_insertf(string_builder *psb, size_t index, double val)
{
    char buf[32];
    if (val > 1e16)
    {
        sprintf(buf, "%e", val);
    }
    else
    {
        sprintf(buf, "%f", val);
    }
    return sb_insert(psb, index, buf);
}

size_t sb_length(string_builder *psb)
{
    if (psb == NULL)
    {
        return 0;
    }
    size_t len = 0;
    _string_builder_item *p = &psb->_root;
    while (p)
    {
        len += p->_len;
        p = p->_next;
    }
    return len;
}

void sb_writeto(string_builder *psb, char *dest)
{
    if (psb == NULL || dest == NULL)
    {
        return;
    }
    char *p = dest;
    _string_builder_item *pitem = &psb->_root;
    while (pitem)
    {
        char *span = _sbi_getval(pitem);
        strcpy(p, span);
        p += pitem->_len;
        pitem = pitem->_next;
    }
}

char *sb_tostring(string_builder *psb)
{
    if (psb == NULL)
    {
        return NULL;
    }
    size_t len = sb_length(psb);
    char *str = (char *)calloc(len + 1, sizeof(char));
    sb_writeto(psb, str);
    return str;
}

string_builder *sb_new(const char *str)
{
    string_builder *ret = (string_builder *)malloc(sizeof(string_builder));
    if (ret != NULL)
    {
        if (!sb_init(ret) || sb_append(ret, str) != ret)
        {
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

void sb_free(string_builder *psb)
{
    if (psb == NULL)
    {
        return;
    }
    sb_clear(psb);
    free(psb);
}

char sb_charat(string_builder *psb, size_t index)
{
    if (psb == NULL)
    {
        return '\0';
    }
    size_t i = 0;
    _string_builder_item *pitem = &psb->_root;
    while (pitem && i + pitem->_len <= index)
    {
        i += pitem->_len;
        pitem = pitem->_next;
    }
    return pitem == NULL
               ? '\0'
               : _sbi_getval(pitem)[index - i];
}

int sb_setcharat(string_builder *psb, size_t index, char c)
{
    if (psb == NULL)
    {
        return 0;
    }
    size_t i = 0;
    _string_builder_item *pitem = &psb->_root;
    while (pitem && i + pitem->_len <= index)
    {
        i += pitem->_len;
        pitem = pitem->_next;
    }
    if (pitem == NULL)
    {
        return 0;
    }
    _sbi_getval(pitem)[index - i] = c;
    return 1;
}

#endif //__STRING_BUILDER_H__
