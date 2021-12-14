#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <Windows.h>

wstring tetromino[7];

// Game area
int nFieldWidth = 12;				// Horizontal size
int nFieldHeight = 18;				// Vertical size
unsigned char *pField = nullptr;	// Types of game cells

// Console screen size
int nScreenWidth = 80;	// X (Columns)
int nScreenHeight = 30;	// Y (Rows)

int Rotate( int px, int py, int r )
{
	switch ( r % 4 )
	{
		case 0: return py * 4 + px;			// 0 degrees
		case 1: return 12 + py - (px * 4);	// 90 degrees
		case 2: return 15 - (py * 4) - px;	// 180 degrees
		case 3: return 3 - py + (px * 4);	// 270 degrees
	}

	return 0;
}

bool DoesPieceFit( int nTetromino, int nRotation, int nPosX, int nPosY )
{
	for ( int px = 0; px < 4; px++ )
		for ( int py = 0; py < 4; py++ )
		{
			// Get index into piece
			int pi = Rotate( px, py, nRotation );

			// Get index into field
			int fi = ( nPosY + py ) * nFieldWidth + ( nPosX + px );

			if ( nPosX + px >= 0 && nPosX + px < nFieldWidth )
			{
				if ( nPosY + py >= 0 && nPosY + py < nFieldHeight )
				{
					if ( tetromino[ nTetromino ][ pi ] == L'X' && pField[ fi ] != 0 )
						return false; // Fail on first hit
				}
			}
		}

	return true;
}

int main()
{
    // Create assets
	// dots = space; X = shape
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

	// Field
	// 0: empty space
	// 9: wall
	pField = new unsigned char[ nFieldWidth * nFieldHeight ]; // Create play field buffer

	for ( int x = 0; x < nFieldWidth; x++ ) // Board boundary
		for ( int y = 0; y < nFieldHeight; y++ )
			pField[ y * nFieldWidth + x ] = ( x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	// Command line as a screen buffer
	wchar_t *screen = new wchar_t[ nScreenWidth * nScreenHeight ];
	for ( int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[ i ] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL ); // Create a new screen buffer
	SetConsoleActiveScreenBuffer( hConsole );
	DWORD dwBytesWritten = 0;

	// Game Logic Stuff
	bool bGameOver = false;

	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];
	bool bRotateHold = false;

	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPieceCount = 0; // Each ten pieces, increase the speed
	int nScore = 0;

	vector< int > vLines;

	// Game loop
	while ( !bGameOver )
	{
		// GAME TIMING ===================================

		this_thread::sleep_for( 50ms ); // Game Tick
		nSpeedCounter++;
		bForceDown = ( nSpeedCounter == nSpeed );

		// INPUT =========================================

		for ( int k = 0; k < 4; k++ )
			// Right, left, down and Z
			bKey[ k ] = ( 0x8000 & GetAsyncKeyState( (unsigned char)( "\x27\x25\x28Z"[ k ] ) ) ) != 0;

		// GAME LOGIC ====================================

		// Right movement
		nCurrentX += ( bKey[ 0 ] && DoesPieceFit( nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY ) ) ? 1 : 0;

		// Left movement
		nCurrentX -= ( bKey[ 1 ] && DoesPieceFit( nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY ) ) ? 1 : 0;

		// Down movement
		nCurrentY += ( bKey[ 2 ] && DoesPieceFit( nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1 ) ) ? 1 : 0;

		// Rotation
		if ( bKey[ 3 ] )
		{
			nCurrentRotation += ( !bRotateHold && DoesPieceFit( nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY ) ) ? 1 : 0;
			bRotateHold = true;
		}
		else
			bRotateHold = false;

		// Force down
		if ( bForceDown )
		{
			if ( DoesPieceFit( nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1 ) )
				nCurrentY++; // It can, so do it!
			else
			{
				// Lock the current piece in the field
				for ( int px = 0; px < 4; px++ )
					for ( int py = 0; py < 4; py++ )
						if ( tetromino[ nCurrentPiece ][ Rotate( px, py, nCurrentRotation ) ] == L'X' )
							pField[ ( nCurrentY + py ) * nFieldWidth + ( nCurrentX + px ) ] = nCurrentPiece + 1;

				// Increase speed
				nPieceCount++;

				if ( nPieceCount % 10 == 0 )
					if ( nSpeed >= 10 ) nSpeed--;

				// Check have we got any lines
				for ( int py = 0; py < 4; py++ )
					if ( nCurrentY + py < nFieldHeight - 1 )
					{
						bool bLine = true;

						for ( int px = 1; px < nFieldWidth - 1; px++ )
							bLine &= pField[ ( nCurrentY + py ) * nFieldWidth + px ] != 0;

						if ( bLine )
						{
							// Remove Line, set to =
							for ( int px = 1; px < nFieldWidth - 1; px++ )
								pField[ ( nCurrentY + py ) * nFieldWidth + px ] = 8;

							vLines.push_back( nCurrentY + py );
						}
					}

				// Set new Score
				nScore += 25;
				if ( !vLines.empty() ) nScore += ( 1 << vLines.size() ) * 100; // Get from 200 to 1'600 points

				// Choose next piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit
				bGameOver = !DoesPieceFit( nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY );
			}

			nSpeedCounter = 0;
		}

		// RENDER OUTPUT =================================

		// Draw Field
		for ( int x = 0; x < nFieldWidth; x++ )
			for ( int y = 0; y < nFieldHeight; y++ )
				screen[ ( y + 2 ) * nScreenWidth + ( x + 2 ) ] = L" ABCDEFG=#"[ pField[ y * nFieldWidth + x ] ];

		// Draw Current Piece
		for ( int px = 0; px < 4; px++ )
			for ( int py = 0; py < 4; py++ )
				if ( tetromino[ nCurrentPiece ][ Rotate( px, py, nCurrentRotation ) ] == L'X' )
					screen[ ( nCurrentY + py + 2 ) * nScreenWidth + ( nCurrentX + px + 2 ) ] = nCurrentPiece + 65;

		// Draw Score
		swprintf_s( &screen[ 2 * nScreenWidth + nFieldWidth + 6 ], 16, L"SCORE: %8d", nScore );

		if ( !vLines.empty() )
		{
			// Display Frame (Cheekily to draw lines)
			WriteConsoleOutputCharacterW( hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten );
			this_thread::sleep_for( 400ms ); // Delay a bit

			for ( auto &v : vLines )
				for ( int px = 1; px < nFieldWidth - 1; px++ )
				{
					// Down all lines
					for ( int py = v; py > 0; py-- )
						pField[ py * nFieldWidth + px ] = pField[ ( py - 1 ) * nFieldWidth + px ];

					pField[ px ] = 0; // For the first line
				}

			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacterW( hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten );
	}

	this_thread::sleep_for( 500ms );

	// Oh Dear
	CloseHandle( hConsole );
	cout << "Game Over!! Score: " << nScore << endl;
	system( "pause" );

    return 0;
}
