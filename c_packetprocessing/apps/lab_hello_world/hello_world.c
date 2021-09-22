#include <nfp.h>
__declspec(ctm) int old[] = {1,2,3,4,5,6,7,8,9,10};
__declspec(ctm) int new[sizeof(old)/sizeof(int)];

int main(void)
{
        if (__ctx() == 0)
        {
                int i, size;
                size = sizeof(old)/sizeof(int);
                for (i=0; i < size; i++)
                {
                        new[i] = old[size - i - 1];
                }
        }
        return 0;
}
