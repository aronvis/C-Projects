#include "RleData.h"
#include <vector>

// Only used more than once for negative runs
void InsertChar(char * data, char value, size_t & charIndex, int & size)
{
    data[charIndex] = value;
    charIndex++;
    size = charIndex;
}

// Used for both positive and negative runs
void InsertFrequency(char * data, char frequency, size_t & frequencyIndex, size_t & charIndex)
{
    data[frequencyIndex] = frequency;
    frequencyIndex = charIndex;
    charIndex = frequencyIndex + 1;
}

void UpdateParameters(char & charFrequency, bool & secondItem, bool & positiveRun)
{
    charFrequency = 1;
    secondItem = true;
    positiveRun = true;
}

void RleData::Compress(const char* input, size_t inSize)
{
    delete [] mData;
    mSize = 0;
    if (inSize > 0)
    {
        mData = new char[inSize*2];
        size_t frequencyIndex = 0;
        size_t charIndex = 1;
        bool secondItem = true;
        bool positiveRun = true;
        bool equalsNextItem = false;
        char charFrequency = 1;
        InsertChar(mData, input[0], charIndex, mSize);
        for(size_t i=1; i<inSize; i++)
        {
            if(secondItem)
            {
                // Positive run of size 2 or greater
                if(input[i] == input[i-1])
                {
                    charFrequency++;
                }
                // Postive run of size 1 has ended
                else if((i < inSize - 1) && (input[i] == input[i+1]))
                {
                    InsertFrequency(mData, charFrequency, frequencyIndex, charIndex);
                    InsertChar(mData,input[i],charIndex,mSize);
                    UpdateParameters(charFrequency, secondItem, positiveRun);
                }
                // Negative run of size 2 or greater
                else
                {
                    positiveRun = false;
                    charFrequency = -2;
                    InsertChar(mData,input[i],charIndex, mSize);
                }
                secondItem = false;
            }
            // Checks if the current run continues
            else
            {
                // Positive Run
                if(positiveRun)
                {
                    // Current run continues
                    if((input[i] == input[i-1]) && (charFrequency < MaxRunSize()))
                    {
                        charFrequency++;
                    }
                    // Current run ends here and another run will start
                    else
                    {
                        // Addes new items to compression array
                        // Moves current available indices of the compression array
                        InsertFrequency(mData, charFrequency, frequencyIndex, charIndex);
                        InsertChar(mData,input[i],charIndex,mSize);
                        UpdateParameters(charFrequency, secondItem, positiveRun);
                    }
                }
                // Negative Run
                else
                {
                    // Checks if the current item is equal to the next item
                    if((i < inSize - 1) && (input[i] == input[i+1]))
                    {
                        equalsNextItem = true;
                    }
                    else
                    {
                        equalsNextItem = false;
                    }
                    // Current run continues
                    if((!equalsNextItem) && ((-1*charFrequency) < MaxRunSize()))
                    {
                        charFrequency--;
                    }
                    // Current run ends
                    else
                    {
                        InsertFrequency(mData, charFrequency, frequencyIndex, charIndex);
                        UpdateParameters(charFrequency, secondItem, positiveRun);
                    }
                    InsertChar(mData,input[i],charIndex,mSize);
                }
            }
        }
        // Adds last frequency to mData
        InsertFrequency(mData, charFrequency, frequencyIndex, charIndex);
    }
}


void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    delete [] mData;
    mSize = outSize;
    mData = new char[mSize];
    int inputIndex = 0;
    int outputIndex = 0;
    while(inputIndex < inSize)
    {
        int count = input[inputIndex];
        inputIndex++;
        // Positive Run
        if(count > 0)
        {
            for(size_t i=0; i<count; i++)
            {
                mData[outputIndex] = input[inputIndex];
                outputIndex++;
            }
            inputIndex++;
        }
        // Negative Run
        else
        {
            count *= -1;
            for(size_t i=0; i<count; i++)
            {
                mData[outputIndex] = input[inputIndex];
                outputIndex++;
                inputIndex++;
            }
        }
    }
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
