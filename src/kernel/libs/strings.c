#include "strings.h"

char *strcpy(char *s1, const char *s2)
{
	strncpy(s1, s2, strlen(s2) + 1);
	s1[strlen(s2)] = '\0'; //tack on the null terminating character if it wasn't already done
	return s1;
}

char *strncpy(char *s1, const char *s2, size_t n)
{
	unsigned int extern_iter = 0; //when s2's length is shorter than n, this allows the function to continue padding null characters

	unsigned int iterator = 0;
	for (iterator = 0; iterator < n; iterator++) //iterate through s2 up to char n, copying them to s1
	{
		if (s2[iterator] != '\0')
			s1[iterator] = s2[iterator];
		else //the end of s2 was found prematurely - copy the null character, update external iterator and quit for loop
		{
			s1[iterator] = s2[iterator];
			extern_iter = iterator + 1;
			break;
		}
	}

	while (extern_iter < n) //while there are still spaces that need to be filled with null characters, fill them
	{
		s1[extern_iter] = '\0';
		extern_iter++;
	}

	return s1;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int count = 0;
	while (count < n)
	{
		if (s1[count] == s2[count])
		{
			if (s1[count] == '\0') //quit early because null-termination found
				return 0;
			else
				count++;
		}
		else
			return s1[count] - s2[count];
	}
	
	return 0;
}

int convert(char s[])
{
    int num = 0;
    int n = strlen(s);
    for (int i = 0; i < n; i++)
        num = num * 10 + (s[i] - 48);
    return num;
}

char *itoa(int nbr)
{
    static char rep[] = "0123456789";
    static char buff[65];
    char        *ptr;
    int         neg;

    ptr = &buff[64];
    *ptr = '\0';
    neg = nbr;
    if (nbr < 0)
        nbr *= -1;
    if (nbr == 0)
        *--ptr = rep[nbr % 10];
    while (nbr != 0)
    {
        *--ptr = rep[nbr % 10];
        nbr /= 10;
    }
    if (neg < 0)
        *--ptr = '-';
    return (ptr);
}