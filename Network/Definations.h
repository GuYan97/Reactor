#ifndef RSVP_DEFINATIONS_H
#define RSVP_DEFINATIONS_H
namespace rsvp
{
/**
 * @brief The head of data pack.
 */
struct head
{
    uint8_t version;
    uint8_t packType;
    uint8_t headLength;
    uint8_t reverse;
    uint32_t dataLength;
    uint32_t crc;
};

/**
 * @brief the type enum of packs
 */
enum PackType
{
    PACK_RESULT,
    PACK_PARAMETER,
    PACK_STATE,
    PACK_PICTURE,
    PACK_CMD,
};

enum CmdType
{
    CMD_PREVIOUS,
    CMD_NEXT,
    CMD_SAVE,
};

enum WorkMode
{
    MODE_WORK,
    MODE_SHOW,
    MODE_DEBUG,
    MODE_SAMPLE,
};
}
#endif //RSVP_DEFINATIONS_H
