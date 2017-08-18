/*
	ar2psemu.c

	By Maff for PSEmu 20/11/98
*/

#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] )
{
	FILE 	*fpIn, *fpOut;
	char	*pValue,Value[100],*x;
	int		mode,y;

	printf("\nar2psemu v1.0.0\n");

	if ( argc != 3 )
	{
		printf(" usage : ar2psemu infile outfile\n\n");
		return 1;
	}

	fpIn = fopen(argv[1],"r");

	if ( !fpIn )
	{
		printf("cant open %s exiting\n",argv[1]);
		return 2;
	}

	fpOut = fopen(argv[2],"w");

	if ( !fpOut )
	{
		fclose( fpIn );
		printf("cant create %s exiting\n",argv[2]);
	}

	pValue = &Value[0];
	mode = 0;

	fputs( "# cheat codes coverted from Action Replay format to PSEMU format by ar2psemu Maff^98\n", fpOut );

    while(1)
	{
		// Main work gets done here

		x = fgets(pValue,80,fpIn);
		if (!x) break;

		if ( Value[0] != '#' )                 			// If its not a comment process it
		{
            if ( Value[0] == 0x22 )						// If its a " then its a game or code
			{
				pValue++;								// Advance one to skip the first speech mark

				for ( y = 1; y < 81; y++ )
				{
					if ( Value[y] == 0x22 )
					{
						Value[y] = 0xA;					// Move a return over top of the speech mark
						Value[y + 1] = 0x00;			// Null terminate the string
						y = 82;							// Jump loop
					}
				}

				if ( mode == 0 )						// If mode is 0 = Game
				{
					mode = 1;							// Set mode to 1 = code for next loop
					fputs( "*S????????|",fpOut );		// Licence ID Not storted, so theres not much we can do
					fputs( pValue,fpOut );				// Wack out the game name
				} else if ( mode == 1 )					// Its a code and the last thing was a game
				{
				 	fputs( "-",fpOut );					// Procede code with a hyphen
					fputs( pValue,fpOut );
				}
				else if ( mode == 2 )					// Its a code and the last hing was a code
				{
					fputs( "--\n",fpOut );				// End of codes
				 	fputs( "-",fpOut );					// Procede code with a hyphen
					fputs( pValue,fpOut );
				}
                                                        // Put the pointer back where its supposed to be
				pValue--;
			}
			else if ( ( ( Value[0] >= 0x30 ) &&			// If its 0 to 9
			    		( Value[0] <= 0x39 ) ) ||
					  ( ( Value[0] >= 0x42 ) &&			// Or A to F
						( Value[0] <= 0x46 ) ) )
			{											// Then its probably a code
				fputs( pValue, fpOut );                 // So just bung it out
				mode = 2;
			}
			else if ( ( Value[0] == '.' ) &&
					  ( Value[1] == 'e' ) &&
					  ( Value[2] == 'n' ) &&
					  ( Value[3] == 'd' ) )				// If start of line = .end
			{
				if ( mode == 2 )
					fputs( "--\n", fpOut );
				fputs( "**\n\n", fpOut );				// End game
				mode = 0;
			}
		} else
		{
			fputs( pValue, fpOut );
		}
	}

	fclose ( fpOut );
	fclose ( fpIn );

	printf("completed sucessfully... bye!\n");
}