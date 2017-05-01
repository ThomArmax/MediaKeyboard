#ifndef OLEDDATA_H
#define OLEDDATA_H

#include <stdlib.h>
#define MAX_OLEDDATA_LEN 128

/**
 * @brief Represents the data to displayed on the OLED screen
 */
class OLEDData
{
public:
    /**
     * @brief Defines the data's type
     */
    enum DataType {
        Undefined = -1, //!< Invalid data type
        Date,           //!< Date
        Time,           //!< Time
        Artist,         //!< Artist
        Album,          //!< Album
        Track,          //!< Track
        Position,       //!< Track's position
        Duration,       //!< Track's duration
        Volume          //!< Player's volume
    };

    explicit OLEDData();
    OLEDData(const DataType type, const char *data);
    ~OLEDData();

    bool operator==(const OLEDData& other);

    DataType type() const;
    void setType(DataType type);

    const char* data() const;
    bool setData(const char *data);

    bool encode(char **pBuffer, int *pSize) const;

    static bool decode(const char* data, int dataSize, OLEDData *decodedData);

private:
    DataType m_type;
    char m_data[MAX_OLEDDATA_LEN];
};

#endif // OLEDDATA_H
