#include <bits/stdc++.h>

#define ll long long int
using namespace std;

#define left 0
#define right 7
#define top 7
#define bottom 0

#define Size 8

// 0 - 7 white Pawn
// 8 - 9 white Rook
// 10 - 11 white Knight
// 12 - 13 white Bishop
// 14 - white Queen
// 15 - white King

// 16 - 23 black Pawn
// 24 - 25 black Rook
// 26 - 27 black Knight
// 28 - 29 black Bishop
// 30 - black King
// 31 - black Queen

pair<string, bool> which_piece(ll num)
{
    if (num >= 0 && num <= 7)
        return {"Pawn", 0};
    else if (num >= 8 && num <= 9)
        return {"Rook", 0};
    else if (num >= 10 && num <= 11)
        return {"Knight", 0};
    else if (num >= 12 && num <= 13)
        return {"Bishop", 0};
    else if (num >= 14 && num <= 14)
        return {"Queen", 0};
    else if (num >= 15 && num <= 15)
        return {"King", 0};
    else if (num >= 16 && num <= 23)
        return {"Pawn", 1};
    else if (num >= 24 && num <= 25)
        return {"Rook", 1};
    else if (num >= 26 && num <= 27)
        return {"Knight", 1};
    else if (num >= 28 && num <= 29)
        return {"Bishop", 1};
    else if (num >= 30 && num <= 30)
        return {"King", 1};
    else if (num >= 31 && num <= 31)
        return {"Queen", 1};
    else
        return {"blank", 0};
}

class Chess;

class Pieces
{
public:
    ll name;     // -1 represents the blank place.
    bool colour; // white -  0 // black - 1
    bool alive;
    bool is_first_move;
    bool is_last_square_move = false;
    pair<ll, ll> position;

    Pieces(ll name, bool colour, pair<ll, ll> position)
    {
        alive = 1;
        this->name = name;
        this->colour = colour;
        this->position = position;
        is_first_move = true;
    }

    virtual bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) {}
    virtual void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) {}
    virtual bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) {}

    bool check_boundry(pair<ll, ll> new_position)
    {
        ll X = new_position.first;
        ll Y = new_position.second;

        if (X >= left && X <= right && Y >= bottom && Y <= top)
            return false;
        else
            return true;
    }
};

class Chess
{
public:
    vector<vector<Pieces *>> chessBoard;
    pair<ll, ll> last_double_step_pawn;
    Chess()
    {
        chessBoard.resize(8, vector<Pieces *>(8, new Pieces(-1, 0, {-1, -1})));
        last_double_step_pawn = make_pair(-1, -1);
    }

    bool check_boundry(pair<ll, ll> new_position)
    {
        ll X = new_position.first;
        ll Y = new_position.second;

        if (X >= left && X <= right && Y >= bottom && Y <= top)
            return false;
        else
            return true;
    }

    bool insufficient_material()
    {
        int white_pieces = 0, black_pieces = 0;
        bool white_has_bishop = false, white_has_knight = false;
        bool black_has_bishop = false, black_has_knight = false;
        bool white_bishop_color = false, black_bishop_color = false;

        // Traverse the chessboard
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Pieces *piece = chessBoard[i][j];

                if (piece->name == -1)
                    continue; // Empty square, skip

                if (which_piece(piece->name).first == "King")
                {
                    continue; // Ignore kings
                }

                if (piece->colour == 0)
                { // White pieces
                    white_pieces++;
                    if (which_piece(piece->name).first == "Bishop")
                    {
                        white_has_bishop = true;
                        white_bishop_color = (i + j) % 2 == 0; // track bishop color (white/black square)
                    }
                    else if (which_piece(piece->name).first == "Knight")
                    {
                        white_has_knight = true;
                    }
                }
                else
                { // Black pieces
                    black_pieces++;
                    if (which_piece(piece->name).first == "Bishop")
                    {
                        black_has_bishop = true;
                        black_bishop_color = (i + j) % 2 == 0; // track bishop color (white/black square)
                    }
                    else if (which_piece(piece->name).first == "Knight")
                    {
                        black_has_knight = true;
                    }
                }
            }
        }

        // Case 1: King vs King
        if (white_pieces == 0 && black_pieces == 0)
            return true;

        // Case 2: King and Bishop vs King
        if (white_pieces == 1 && white_has_bishop && black_pieces == 0)
            return true;
        if (black_pieces == 1 && black_has_bishop && white_pieces == 0)
            return true;

        // Case 3: King and Knight vs King
        if (white_pieces == 1 && white_has_knight && black_pieces == 0)
            return true;
        if (black_pieces == 1 && black_has_knight && white_pieces == 0)
            return true;

        // Case 4: King and Bishop vs King and Bishop (same color)
        if (white_pieces == 1 && black_pieces == 1 && white_has_bishop && black_has_bishop && white_bishop_color == black_bishop_color)
        {
            return true;
        }

        // No insufficient material scenario
        return false;
    }
};

void print_chessboard(vector<vector<Pieces *>> &chessBoard)
{
    cout << "Current Chessboard State:" << endl;

    // Print the top border
    cout << "  +-------------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;

    for (int x = 7; x >= 0; x--)
    {                      // Printing from top (row 7) to bottom (row 0)
        cout << x << " |"; // Row number on the left side

        for (int y = 0; y < 8; y++)
        {
            Pieces *piece = chessBoard[x][y];
            cout << "[" << x << " " << y << "]";
            if (piece->name == -1)
            {
                cout << "             |"; // Empty square
            }
            else
            {
                // Determine the color and type of piece
                string color = (piece->colour == 0) ? "White" : "Black";
                string pieceType;

                // Map the name to the piece type
                switch (piece->name)
                {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    pieceType = "Pawn  |";
                    break;
                case 8:
                case 9:
                    pieceType = "Rook  |";
                    break;
                case 10:
                case 11:
                    pieceType = "Knight|";
                    break;
                case 12:
                case 13:
                    pieceType = "Bishop|";
                    break;
                case 14:
                    pieceType = "Queen |";
                    break;
                case 15:
                    pieceType = "King  |";
                    break;
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                    pieceType = "Pawn  |";
                    break;
                case 24:
                case 25:
                    pieceType = "Rook  |";
                    break;
                case 26:
                case 27:
                    pieceType = "Knight|";
                    break;
                case 28:
                case 29:
                    pieceType = "Bishop|";
                    break;
                case 30:
                    pieceType = "King  |";
                    break;
                case 31:
                    pieceType = "Queen |";
                    break;
                default:
                    pieceType = "Unknown|";
                    break;
                }

                cout << " " << color << " " << pieceType; // Print the piece with color
            }
        }
        cout << endl; // Close the row with a border
        cout << "  +-------------------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
    }

    cout << "            0                  1                   2                 3                  4                  5                  6                  7" << endl; // Column labels
    cout << endl;
}

class King : public Pieces
{
public:
    King(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position) {}

    bool is_check(vector<vector<Pieces *>> &chessBoard, pair<ll, ll> position)
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        // Bishop - Queen
        bool top_right = false;
        bool top_left = false;
        bool bottom_left = false;
        bool bottom_right = false;

        // Rook - Queen
        bool Top = false;
        bool Bottom = false;
        bool Left = false;
        bool Right = false;

        for (int i = 1; i <= Size; i++)
        {
            int j = i;
            if (!top_right && !check_boundry({curr_X + i, curr_Y + j}))
            {
                if (chessBoard[curr_X + i][curr_Y + j]->name != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y + j]->colour)
                    {
                        ll new_name = chessBoard[curr_X + i][curr_Y + j]->name;
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            top_right = true;
                    }
                    else
                        top_right = true;
                }
            }

            if (!top_left && !check_boundry({curr_X - i, curr_Y + j}))
            {
                if (chessBoard[curr_X - i][curr_Y + j]->name != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y + j]->colour)
                    {
                        ll new_name = chessBoard[curr_X - i][curr_Y + j]->name;
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            top_left = true;
                    }
                    else
                        top_left = true;
                }
            }

            if (!bottom_right && !check_boundry({curr_X + i, curr_Y - j}))
            {
                if (chessBoard[curr_X + i][curr_Y - j]->name != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y - j]->colour)
                    {
                        ll new_name = chessBoard[curr_X + i][curr_Y - j]->name;
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            bottom_right = true;
                    }
                    else
                        bottom_right = true;
                }
            }

            if (!bottom_left && !check_boundry({curr_X - i, curr_Y - j}))
            {
                if (chessBoard[curr_X - i][curr_Y - j]->name != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y - j]->colour)
                    {
                        ll new_name = chessBoard[curr_X - i][curr_Y - j]->name;
                        if (new_name == 12 || new_name == 13 || new_name == 28 || new_name == 29 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            bottom_left = true;
                    }
                    else
                        bottom_left = true;
                }
            }

            if (!Right && !check_boundry({curr_X + i, curr_Y}))
            {
                if (chessBoard[curr_X + i][curr_Y]->name != -1)
                {
                    if (colour != chessBoard[curr_X + i][curr_Y]->colour)
                    {
                        ll new_name = chessBoard[curr_X + i][curr_Y]->name;
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Right = true;
                    }
                    else
                        Right = true;
                }
            }

            if (!Left && !check_boundry({curr_X - i, curr_Y}))
            {
                if (chessBoard[curr_X - i][curr_Y]->name != -1)
                {
                    if (colour != chessBoard[curr_X - i][curr_Y]->colour)
                    {
                        ll new_name = chessBoard[curr_X - i][curr_Y]->name;
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Left = true;
                    }
                    else
                        Left = true;
                }
            }

            if (!Top && !check_boundry({curr_X, curr_Y + i}))
            {
                if (chessBoard[curr_X][curr_Y + i]->name != -1)
                {
                    if (colour != chessBoard[curr_X][curr_Y + i]->colour)
                    {
                        ll new_name = chessBoard[curr_X][curr_Y + i]->name;
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Top = true;
                    }
                    else
                        Top = true;
                }
            }

            if (!Bottom && !check_boundry({curr_X, curr_Y - i}))
            {
                if (chessBoard[curr_X][curr_Y - i]->name != -1)
                {
                    if (colour != chessBoard[curr_X][curr_Y - i]->colour)
                    {
                        ll new_name = chessBoard[curr_X][curr_Y - i]->name;
                        if (new_name == 8 || new_name == 9 || new_name == 24 || new_name == 25 || new_name == 31 || new_name == 14)
                        {
                            return true;
                        }
                        else
                            Bottom = true;
                    }
                    else
                        Bottom = true;
                }
            }
        }

        // pawn
        bool pawn_threat = false;
        if (colour == 0)
        {
            if ((!check_boundry({curr_X + 1, curr_Y + 1}) && (chessBoard[curr_X + 1][curr_Y + 1]->name >= 16 && chessBoard[curr_X + 1][curr_Y + 1]->name <= 23)) || (!check_boundry({curr_X - 1, curr_Y + 1}) && (chessBoard[curr_X - 1][curr_Y + 1]->name >= 16 && chessBoard[curr_X - 1][curr_Y + 1]->name <= 23)))
                pawn_threat = true;
        }
        else
        {
            if ((!check_boundry({curr_X + 1, curr_Y - 1}) && (chessBoard[curr_X + 1][curr_Y - 1]->name >= 0 && chessBoard[curr_X + 1][curr_Y - 1]->name <= 7)) || (!check_boundry({curr_X - 1, curr_Y - 1}) && (chessBoard[curr_X - 1][curr_Y - 1]->name >= 0 && chessBoard[curr_X - 1][curr_Y - 1]->name <= 7)))
                pawn_threat = true;
        }
        if (pawn_threat)
            return true;

        // Knight

        bool knight_threat = false;

        int row[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        int col[8] = {1, -1, -1, 1, 2, -2, 2, -2};

        for (int i = 0; i < 8; i++)
        {
            if (!check_boundry({curr_X + row[i], curr_Y + col[i]}))
            {
                ll new_name = chessBoard[curr_X + row[i]][curr_Y + col[i]]->name;
                if (!knight_threat && new_name != -1 && colour != chessBoard[curr_X + row[i]][curr_Y + col[i]]->colour && (new_name == 10 || new_name == 11 || new_name == 26 || new_name == 27))
                    knight_threat = true;
            }
        }
        if (knight_threat)
            return true;

        // if none of the above check condition returns true then there is no check so we can return false;
        return false;
    }

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        bool curr_colour = this->colour;
        if (chessBoard[new_X][new_Y]->name == -1)
            return false;
        else if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != colour)
            return false;
        else
            return true;
    }

    bool castling_condition(pair<ll, ll> &new_position, vector<vector<Pieces *>> &chessBoard)
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (colour == 0 && curr_X == 0 && curr_Y == 4 && abs(new_Y - curr_Y) == 2 && abs(new_X - curr_X) == 0) // castling condition  // keep white rook named 9 right side \\ keep white rook named 8 at left side
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i <= 2; i++)
                {
                    if (chessBoard[curr_X][curr_Y + i]->name != -1)
                        return false;
                }
                if (chessBoard[curr_X][curr_Y + 3]->name != 9 || chessBoard[curr_X][curr_Y + 3]->is_first_move == false)
                    return false;
            }
            else if (new_Y < curr_Y)
            {
                for (int i = 1; i <= 3; i++)
                {
                    if (chessBoard[curr_X][curr_Y - i]->name != -1)
                        return false;
                }
                if (chessBoard[curr_X][curr_Y - 4]->name != 8 || chessBoard[curr_X][curr_Y - 4]->is_first_move == false)
                    return false;
            }
            return true;
        }
        else if (colour == 1 && curr_Y == 4 && curr_X == 7 && abs(new_Y - curr_Y) == 2 && abs(new_X - curr_X) == 0)
        {
            if (new_Y > curr_Y)
            {
                for (int i = 1; i <= 2; i++)
                {
                    if (chessBoard[curr_X][curr_Y + i]->name != -1)
                        return false;
                }
                if (chessBoard[curr_X][curr_Y + 3]->name != 25 || chessBoard[curr_X][curr_Y + 3]->is_first_move == false)
                    return false;
            }
            else if (new_Y < curr_Y)
            {
                for (int i = 1; i <= 3; i++)
                {
                    if (chessBoard[curr_X][curr_Y - i]->name != -1)
                        return false;
                }
                if (chessBoard[curr_X][curr_Y - 4]->name != 24 || chessBoard[curr_X][curr_Y - 4]->is_first_move == false)
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (alive == false || check_boundry(new_position))
            return false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (abs(new_X - curr_X) <= 1 && abs(new_Y - curr_Y) <= 1 && abs(new_X - curr_X) + abs(new_Y - curr_Y) != 0)
        {
            if (check_obstacle(new_position, chessBoard))
            {
                return false;
            }
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
        else if (castling_condition(new_position, chessBoard)) // castling condition
        {
            move_position(new_position, chessBoard);
            return true;
        }
        else
            return false;
    }

    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (is_first_move == true)
            is_first_move = false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (abs(new_Y - curr_Y) == 2) //  must be castling
        {
            Pieces *currPiece = chessBoard[new_X][new_Y];
            if (currPiece->name != -1)
                currPiece->alive = false;

            // delete currPiece;
            chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
            position = new_position;
            chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});

            Pieces *rook;
            Pieces *newplace;
            if (curr_X == 0 && new_Y == 6)
            {
                rook = chessBoard[0][7];
                newplace = chessBoard[0][5];
                chessBoard[0][5] = rook;
                rook->position = {0, 5};
                chessBoard[0][7] = newplace;
            }
            else if (curr_X == 0 && new_Y == 2)
            {
                rook = chessBoard[0][0];
                newplace = chessBoard[0][3];
                chessBoard[0][3] = rook;
                rook->position = {0, 3};
                chessBoard[0][0] = newplace;
            }
            else if (curr_X == 7 && new_Y == 6)
            {
                rook = chessBoard[7][7];
                newplace = chessBoard[7][5];
                chessBoard[7][5] = rook;
                rook->position = {7, 5};
                chessBoard[7][7] = newplace;
            }
            else if (curr_X == 7 && new_Y == 2)
            {
                rook = chessBoard[7][0];
                newplace = chessBoard[7][3];
                chessBoard[7][3] = rook;
                rook->position = {7, 3};
                chessBoard[7][0] = newplace;
            }
        }
        else
        {
            Pieces *currPiece = chessBoard[new_X][new_Y];
            if (currPiece->name != -1)
                currPiece->alive = false;

            // delete currPiece;
            chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
            position = new_position;
            chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});
        }
    }

    bool is_checkmate(vector<vector<Pieces *>> &chessBoard)
    {
        // Chess temp = game;

        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                if (chessBoard[i][j]->colour == colour)
                {
                    for (int k = 0; k < Size; k++)
                    {
                        for (int l = 0; l < Size; l++)
                        {
                            if (i == k && l == j)
                                continue;
                            Pieces *p = chessBoard[i][j];
                            bool is_first_move = p->is_first_move;
                            bool is_last_square_move = p->is_last_square_move;
                            Pieces *new_piece = chessBoard[k][l];
                            // Chess temp2 = temp;
                            if (chessBoard[i][j]->is_valid_move({k, l}, chessBoard))
                            {
                                if (!is_check(chessBoard, position))
                                {
                                    chessBoard[i][j] = p;
                                    p->position = {i, j};
                                    p->is_first_move = is_first_move;
                                    p->is_last_square_move = is_last_square_move;
                                    chessBoard[k][l] = new_piece;
                                    new_piece->alive = true;
                                    return false;
                                }

                                chessBoard[i][j] = p;
                                p->position = {i, j};
                                p->is_first_move = is_first_move;
                                p->is_last_square_move = is_last_square_move;
                                chessBoard[k][l] = new_piece;
                                new_piece->alive = true;
                            }
                            // temp = temp2;
                        }
                    }
                }
            }
        }

        // if function not returened yet false then is means that is checkmate hence return true
        return true;
    }

    bool is_stalemate(vector<vector<Pieces *>> &chessBoard, bool colour)
    {

        if (is_check(chessBoard, position))
        {
            return false;
        }

        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                if (chessBoard[i][j]->colour == colour)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        for (int l = 0; l < 8; l++)
                        {
                            if (i == k && l == j)
                                continue;
                            Pieces *p = chessBoard[i][j];
                            bool is_first_move = p->is_first_move;
                            bool is_last_square_move = p->is_last_square_move;
                            Pieces *new_piece = chessBoard[k][l];
                            if (chessBoard[i][j]->is_valid_move({k, l}, chessBoard))
                            {
                                if (is_check(chessBoard, position))
                                {
                                    chessBoard[i][j] = p;
                                    p->position = {i, j};
                                    p->is_first_move = is_first_move;
                                    p->is_last_square_move = is_last_square_move;
                                    chessBoard[k][l] = new_piece;
                                    new_piece->alive = true;
                                    continue;
                                }

                                chessBoard[i][j] = p;
                                p->position = {i, j};
                                p->is_first_move = is_first_move;
                                p->is_last_square_move = is_last_square_move;
                                chessBoard[k][l] = new_piece;
                                new_piece->alive = true;
                                return false;
                            }
                        }
                    }
                }
            }
        }

        // if function not returened yet false then is means that is stalemate hence return true
        return true;
    }
};

class Rook : public Pieces
{
public:
    Rook(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position) {}

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (new_X == curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = curr_Y + 1; i < new_Y; i++)
                {
                    if (chessBoard[curr_X][i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = curr_Y - 1; i > new_Y; i--)
                {
                    if (chessBoard[curr_X][i]->name != -1)
                        return true;
                }
            }
        }
        else if (new_Y == curr_Y)
        {
            if (new_X > curr_X)
            {
                for (ll i = curr_X + 1; i < new_X; i++)
                {
                    if (chessBoard[i][curr_Y]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = curr_X - 1; i > new_X; i--)
                {
                    if (chessBoard[i][curr_Y]->name != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != colour)
            return false;
        else if (chessBoard[new_X][new_Y]->name != -1)
            return true;
        else
            return false;
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (new_X != curr_X && new_Y != curr_Y)
            return false;
        else if (new_X == curr_X && new_Y == curr_Y)
            return false;
        else
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
    }
    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (is_first_move == true)
            is_first_move = false;
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        Pieces *currPiece = chessBoard[new_X][new_Y];
        if (currPiece->name != -1)
            currPiece->alive = false;

        // delete currPiece;
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});
    }
};

class Bishop : public Pieces
{
public:
    Bishop(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position) {}

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        ll diff = abs(new_X - curr_X);

        if (new_X > curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y + i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y - i]->name != -1)
                        return true;
                }
            }
        }
        else if (new_X < curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y + i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y - i]->name != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour)
            return false;
        else if (chessBoard[new_X][new_Y]->name != -1)
            return true;
        else
            return false;
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if ((abs(new_X - curr_X) != abs(new_Y - curr_Y)) || (abs(new_X - curr_X) == 0))
            return false;
        else
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
    }
    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        Pieces *currPiece = chessBoard[new_X][new_Y];
        if (currPiece->name != -1)
            currPiece->alive = false;

        // delete currPiece;
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});
    }
};

class Knight : public Pieces
{
public:
    Knight(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position) {}

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour)
            return false;
        else if (chessBoard[new_X][new_Y]->name != -1)
            return true;
        else
            return false;
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        int row[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        int col[8] = {1, -1, -1, 1, 2, -2, 2, -2};

        for (int i = 0; i < 8; i++)
        {
            if (curr_X + row[i] == new_X && curr_Y + col[i] == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                else
                {
                    move_position(new_position, chessBoard);
                    return true;
                }
            }
        }
        return false;
    }
    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        Pieces *currPiece = chessBoard[new_X][new_Y];
        if (currPiece->name != -1)
            currPiece->alive = false;

        // delete currPiece;
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});
    }
};

class Queen : public Pieces
{
public:
    Queen(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position) {}

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        ll diff = abs(new_X - curr_X);

        if (new_X == curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = curr_Y + 1; i < new_Y; i++)
                {
                    if (chessBoard[curr_X][i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = curr_Y - 1; i > new_Y; i--)
                {
                    if (chessBoard[curr_X][i]->name != -1)
                        return true;
                }
            }
        }
        else if (new_Y == curr_Y)
        {
            if (new_X > curr_X)
            {
                for (ll i = curr_X + 1; i < new_X; i++)
                {
                    if (chessBoard[i][curr_Y]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = curr_X - 1; i > new_X; i--)
                {
                    if (chessBoard[i][curr_Y]->name != -1)
                        return true;
                }
            }
        }
        else if (new_X > curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y + i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X + i][curr_Y - i]->name != -1)
                        return true;
                }
            }
        }
        else if (new_X < curr_X)
        {
            if (new_Y > curr_Y)
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y + i]->name != -1)
                        return true;
                }
            }
            else
            {
                for (ll i = 1; i < diff; i++)
                {
                    if (chessBoard[curr_X - i][curr_Y - i]->name != -1)
                        return true;
                }
            }
        }

        if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour)
            return false;
        else if (chessBoard[new_X][new_Y]->name != -1)
            return true;
        else
            return false;
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (this->alive == false || check_boundry(new_position))
            return false;

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (new_X == curr_X && new_Y == curr_Y) // not a same place
            return false;
        else if ((abs(new_X - curr_X) == abs(new_Y - curr_Y)) || (new_X == curr_X || new_Y == curr_Y)) // bishop or Rook
        {
            if (check_obstacle(new_position, chessBoard))
                return false;
            else
            {
                move_position(new_position, chessBoard);
                return true;
            }
        }
        else
            return false;
    }
    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        Pieces *currPiece = chessBoard[new_X][new_Y];
        if (currPiece->name != -1)
            currPiece->alive = false;

        // delete currPiece;
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});
    }
};

class Pawn : public Pieces
{
public:
    Pawn(ll name, bool colour, pair<ll, ll> position) : Pieces(name, colour, position)
    {
        is_last_square_move = false;
    }

    bool check_obstacle(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        // for white
        if (!colour)
        {
            if (new_Y == curr_Y)
            {
                if ((new_X - curr_X == 2) && (chessBoard[new_X][new_Y]->name != -1 || chessBoard[new_X - 1][new_Y]->name != -1))
                    return true;
                else if (new_X - curr_X == 1 && chessBoard[new_X][new_Y]->name != -1)
                {
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour)
                    return true;
                else if (chessBoard[new_X - 1][new_Y]->name != -1 && chessBoard[new_X - 1][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour && chessBoard[new_X][new_Y]->name == -1 && chessBoard[new_X - 1][new_Y]->is_last_square_move == 1)
                {
                    return true;
                }
                else
                    return false;
            }
        }
        else // for black
        {
            if (new_Y == curr_Y)
            {
                if (((curr_X - new_X) == 2) && (chessBoard[new_X][new_Y]->name != -1 || chessBoard[new_X + 1][new_Y]->name != -1))
                    return true;
                else if (curr_X - new_X == 1 && chessBoard[new_X][new_Y]->name != -1)
                    return true;
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if (chessBoard[new_X][new_Y]->name != -1 && chessBoard[new_X][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour)
                    return true;
                else if (chessBoard[new_X + 1][new_Y]->name != -1 && chessBoard[new_X + 1][new_Y]->colour != chessBoard[curr_X][curr_Y]->colour && chessBoard[new_X][new_Y]->name == -1 && chessBoard[new_X + 1][new_Y]->is_last_square_move == 1)
                {
                    return true;
                }
                else
                    return false;
            }
        }
    }

    bool is_valid_move(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        if (check_boundry(new_position))
        {
            return false;
        }

        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        // for white
        if (!colour)
        {
            if (curr_Y == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                if (is_first_move && new_X - curr_X == 2)
                {
                    is_first_move = false;
                    is_last_square_move = true;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else if (new_X - curr_X == 1)
                {
                    if (is_first_move)
                        is_first_move = false;
                    is_last_square_move = false;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if ((new_X - curr_X) != 1)
                    return false;
                else if (check_obstacle(new_position, chessBoard))
                {
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else // for black
        {
            if (curr_Y == new_Y)
            {
                if (check_obstacle(new_position, chessBoard))
                    return false;
                if (is_first_move && curr_X - new_X == 2)
                {
                    is_first_move = false;
                    is_last_square_move = true;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else if (curr_X - new_X == 1)
                {
                    if (is_first_move)
                        is_first_move = false;
                    is_last_square_move = false;
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else if (abs(new_Y - curr_Y) == 1)
            {
                if ((curr_X - new_X) != 1)
                    return false;
                else if (check_obstacle(new_position, chessBoard))
                {
                    move_position(new_position, chessBoard);
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }

    void move_position(pair<ll, ll> new_position, vector<vector<Pieces *>> &chessBoard) override
    {
        ll curr_X = position.first;
        ll curr_Y = position.second;

        ll new_X = new_position.first;
        ll new_Y = new_position.second;

        if (abs(new_Y - curr_Y) == 1 && chessBoard[new_X][new_Y]->name == -1) // must be En passant
        {
            chessBoard[curr_X][new_Y] = new Pieces(-1, 0, {-1, -1});
        }

        Pieces *currPiece = chessBoard[new_X][new_Y];
        if (currPiece->name != -1)
            currPiece->alive = false;

        // delete currPiece;
        chessBoard[new_X][new_Y] = chessBoard[curr_X][curr_Y];
        position = new_position;
        chessBoard[curr_X][curr_Y] = new Pieces(-1, 0, {-1, -1});

        bool choice = true;

        while (choice && (colour == 0 && new_X == 7) || (colour == 1 && new_X == 0))
        {
            print_chessboard(chessBoard);
            cout << "Hurray We have some bonus for you Enter the options which ever you want to upgrade this pawn to : " << endl;
            cout << "Queen Rook Knight Bishop None" << endl;
            cout << "enter the name : ";
            string s;
            cin >> s;
            if (s == "Queen")
            {
                int new_name = (colour == 0) ? 14 : 31;
                chessBoard[new_X][new_Y]->alive = false;
                chessBoard[new_X][new_Y] = new Queen(new_name, colour, position);
                choice = false;
            }
            else if (s == "Knight")
            {
                int new_name = (colour == 0) ? 10 : 26;
                chessBoard[new_X][new_Y]->alive = false;
                chessBoard[new_X][new_Y] = new Knight(new_name, colour, position);
                choice = false;
            }
            else if (s == "Bishop")
            {
                int new_name = (colour == 0) ? 12 : 28;
                chessBoard[new_X][new_Y]->alive = false;
                chessBoard[new_X][new_Y] = new Bishop(new_name, colour, position);
                choice = false;
            }
            else if (s == "Rook")
            {
                int new_name = (colour == 0) ? 8 : 24;
                chessBoard[new_X][new_Y]->alive = false;
                chessBoard[new_X][new_Y] = new Queen(new_name, colour, position);
                choice = false;
            }
            else if (s == "None")
            {
                choice = false;
            }
            else
            {
                choice = true;
                cout << "invalid choice" << endl;
            }
        }
    }
};

void fn()
{
    Chess c;

    ll chessBoard[8][8] = {
        {8, 10, 12, 14, 15, 13, 11, 9},   // First row: White's main pieces (Rook, Knight, Bishop, Queen, King, etc.)
        {0, 1, 2, 3, 4, 5, 6, 7},         // Second row: White's pawns
        {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
        {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
        {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
        {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
        {16, 17, 18, 19, 20, 21, 22, 23}, // Seventh row: Black's pawns
        {24, 26, 28, 31, 30, 29, 27, 25}  // Eighth row: Black's main pieces (Rook, Knight, Bishop, Queen, King, etc.)
    };

    // // stalemate example
    // ll chessBoard[8][8] = {
    //     {30, -1, -1, -1, -1, -1, -1, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, -1, -1}, // Second row: White's pawns
    //     {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, 31, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, -1, -1}, // Empty row
    //     {-1, -1, -1, -1, -1, -1, -1, 15}  // Eighth row: Black's main pieces (Rook, Knight, Bishop, Queen, King, etc.)
    // };

    King *white_king;
    King *black_king;

    for (ll i = 0; i < Size; i++)
    {
        for (ll j = 0; j < Size; j++)
        {
            auto a = which_piece(chessBoard[i][j]);
            string Name = a.first;
            bool colour = a.second;
            pair<ll, ll> position = make_pair(i, j);

            if (Name == "blank")
            {
                c.chessBoard[i][j] = new Pieces(chessBoard[i][j], colour, position);
            }
            else if (Name == "Pawn")
            {
                c.chessBoard[i][j] = new Pawn(chessBoard[i][j], colour, position);
            }
            if (Name == "King")
            {
                if (colour)
                {
                    black_king = new King(chessBoard[i][j], colour, position);
                }
                else
                {
                    white_king = new King(chessBoard[i][j], colour, position);
                }
                c.chessBoard[i][j] = (colour == 0) ? white_king : black_king;
            }
            else if (Name == "Queen")
            {
                c.chessBoard[i][j] = new Queen(chessBoard[i][j], colour, position);
            }
            else if (Name == "Bishop")
            {
                c.chessBoard[i][j] = new Bishop(chessBoard[i][j], colour, position);
            }
            else if (Name == "Knight")
            {
                c.chessBoard[i][j] = new Knight(chessBoard[i][j], colour, position);
            }
            else if (Name == "Rook")
            {
                c.chessBoard[i][j] = new Rook(chessBoard[i][j], colour, position);
            }
        }
    }
    print_chessboard(c.chessBoard);

    bool color = 0;
    while (1)
    {
        cout << "Turn " << (color == 0 ? "white" : "black") << endl;

        // check stalemate
        if ((color == 0 && white_king->is_stalemate(c.chessBoard, color)) || (color == 1 && black_king->is_stalemate(c.chessBoard, color)))
        {
            cout << "Draw due to stalemate.....!!";
            break;
        }

        // check checkmate
        if ((color == 0 && white_king->is_check(c.chessBoard, white_king->position)) || (color == 1 && black_king->is_check(c.chessBoard, black_king->position)))
        {
            if ((color == 0 && white_king->is_checkmate(c.chessBoard)) || (color == 1 && black_king->is_checkmate(c.chessBoard)))
            {
                string ans = (color == 1) ? "white" : "black";
                cout << "There is a check mate" << ans << "is a winner" << endl;
                break;
            }
            cout << "There is a check" << endl;
        }

        // check insufficient material
        if (c.insufficient_material())
        {
            cout << "Draw due to insuffiecient material.....!!" << endl;
            break;
        }

        ll curr_X, curr_Y;
        cout << "Please enter the valid piece : ";
        cin >> curr_X >> curr_Y;

        ll new_X, new_Y;
        cout << "Please enter the valid place : ";
        cin >> new_X >> new_Y;

        if (c.check_boundry({curr_X, curr_Y}) || c.chessBoard[curr_X][curr_Y]->name == -1)
        {
            cout << "Invalid starting piece" << endl;
            continue;
        }

        pair<ll, ll> new_position = make_pair(new_X, new_Y);
        if (c.check_boundry(new_position))
        {
            cout << "Invalid new position" << endl;
            continue;
        }

        Pieces *p = c.chessBoard[curr_X][curr_Y];
        bool is_first_move = p->is_first_move;
        bool is_last_square_move = p->is_last_square_move;
        if (color != p->colour)
        {
            cout << "Invalid starting piece" << endl;
            continue;
        }

        Pieces *new_piece = c.chessBoard[new_X][new_Y];
        if (!(p->is_valid_move(new_position, c.chessBoard)))
        {
            cout << "Invalid new place" << endl;
            continue;
        }
        if ((!color && white_king->is_check(c.chessBoard, white_king->position)) || (color && black_king->is_check(c.chessBoard, black_king->position)))
        {
            c.chessBoard[curr_X][curr_Y] = p;
            p->position = {curr_X, curr_Y};
            p->is_first_move = is_first_move;
            p->is_last_square_move = is_last_square_move;
            c.chessBoard[new_X][new_Y] = new_piece;
            new_piece->alive = true;

            if (white_king->is_checkmate(c.chessBoard))
            {
                string ans = (color == 0) ? "White" : "Black";
                cout << "Oh no, " << ans << " is loser...........!!" << endl;
                break;
            }
            cout << "Opps...!! there is a check" << endl;
            continue;
        }
        print_chessboard(c.chessBoard);
        color = !color;
    }
}

int main()
{
    // Programmed by By PARTH;

    int t = 1;
    // cin >> t;
    while (t--)
    {
        fn();
    }
    return 0;
}