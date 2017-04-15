#include "oleddata.h"
#include <string.h>
#include <stdint.h>

//_________________________________________________________________
/**
 * @brief Construct a new OLEDData
 */
OLEDData::OLEDData()
    : m_type(Undefined)
    , m_data(NULL)
{

}

//_________________________________________________________________
/**
 * @brief Destroy the OLEDData of frees the allocated ressources
 */
OLEDData::~OLEDData()
{
    if (m_data)
        free(m_data);
}

//_________________________________________________________________
/**
 * @brief OLEDData::operator ==
 * @param other
 * @return true if equals
 */
bool OLEDData::operator ==(const OLEDData& other)
{
    return ((other.type() == type()) &&
            (strcmp(other.data(), data()) == 0));
}

//_________________________________________________________________
/**
 * @brief Returns the data type
 * @return the data type
 * @sa setType()
 */
OLEDData::DataType OLEDData::type() const
{
    return m_type;
}

//_________________________________________________________________
/**
 * @brief Sets the data's type
 * @param type
 * @sa type()
 */
void OLEDData::setType(DataType type)
{
    m_type = type;
}

//_________________________________________________________________
/**
 * @brief Returns the data
 * @return the data
 * @sa setData()
 */
const char *OLEDData::data() const
{
    return m_data;
}

//_________________________________________________________________
/**
 * @brief Sets the data
 * @param data
 * @return true if succeeded; false otherwise
 * @sa data()
 * @note data MUST be null terminated
 */
bool OLEDData::setData(const char *data)
{
    size_t dataSize = strlen(data);

    if (m_data)
        free(m_data);

    m_data = static_cast<char*>(malloc(dataSize+1));
    if (!m_data)
        return false;

    memset(m_data, 0, dataSize);
    strcpy(m_data, data);

    return true;
}

//_________________________________________________________________
/**
 * @brief Encodes the OLEDData
 * @param[out] pBuffer  Encoded data
 * @param[out] pSize    Encoded data's length
 * @return true if succeeded; false otherwise
 */
bool OLEDData::encode(char **pBuffer, int *pSize) const
{
    if (!pBuffer)
        return false;

    if (!m_data)
        return false;

    int dataSize = strlen(m_data);
    int sizeAllocated = 1 + 1 + dataSize + 1;

    // Allocate memory
    char *lpBuffer = static_cast<char*>(malloc(sizeAllocated * sizeof(char)));
    if (!lpBuffer)
        return false;

    // Add the DataType on 1 byte
    lpBuffer[0] = static_cast<uint8_t>(m_type);
    // Add the data size on 1 byte
    lpBuffer[1] = static_cast<uint8_t>(dataSize);
    // Copy the data into the buffer
    //memcpy(lpBuffer+2, m_data, dataSize);
    strcpy(lpBuffer+2, m_data);

    if (pSize)
        *pSize = sizeAllocated;

    *pBuffer = lpBuffer;
    return true;
}

//_________________________________________________________________
/**
 * @brief Sets the OLEDData with the given data
 * @param[in]  data         Data to be decoded
 * @param[in]  dataSize     Data's size
 * @param[out] decodedData  The OLEDData to be populated
 * @return true if the decoding was successful; false otherwise
 */
bool OLEDData::decode(const char *data, int dataSize, OLEDData *decodedData)
{
    if (!data || dataSize < 2 || !decodedData)
        return false;

    decodedData->setType(static_cast<DataType>(data[0]));

    int size = data[1];

    // Extract data
    char *extractedData = static_cast<char*>(malloc((size * sizeof(char))+1));
    if (!extractedData)
        return false;
    memset(extractedData, 0, size+1);
    memcpy(extractedData, data+2, size);

    decodedData->setData(extractedData);

    free(extractedData);

    return true;
}
