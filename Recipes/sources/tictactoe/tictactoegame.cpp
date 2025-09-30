#include "tictactoegame.h"


TicTacToeGame::TicTacToeGame(QObject *parent)
    : QObject(parent), m_board(9, 0), m_currentPlayer(1), m_gameOver(false), m_winner(0)
{
}


QVariantList TicTacToeGame::board() const
{
    QVariantList list;
    for (int v : m_board) list << v;
    return list;
}


int TicTacToeGame::currentPlayer() const { return m_currentPlayer; }
bool TicTacToeGame::gameOver() const { return m_gameOver; }
int TicTacToeGame::winner() const { return m_winner; }


bool TicTacToeGame::makeMove(int index)
{
    if (index < 0 || index >= 9) return false;
    if (m_gameOver) return false;
    if (m_board[index] != 0) return false;


    m_board[index] = m_currentPlayer;
    emit boardChanged();


    checkGameState();
    if (!m_gameOver) {
        m_currentPlayer = -m_currentPlayer;
        emit currentPlayerChanged();
    }
    return true;
}


void TicTacToeGame::reset()
{
    m_board.fill(0);
    m_currentPlayer = 1;
    m_gameOver = false;
    m_winner = 0;
    emit boardChanged();
    emit currentPlayerChanged();
    emit gameOverChanged();
    emit winnerChanged();
}


void TicTacToeGame::checkGameState()
{
    static const int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };


    for (auto &line : lines) {
        int a = m_board[line[0]];
        if (a != 0 && a == m_board[line[1]] && a == m_board[line[2]]) {
            m_gameOver = true;
            m_winner = a;
            emit gameOverChanged();
            emit winnerChanged();
            return;
        }
    }


    bool hasEmpty = false;
    for (int v : m_board) if (v == 0) { hasEmpty = true; break; }
    if (!hasEmpty) {
        m_gameOver = true;
        m_winner = 0; // draw
        emit gameOverChanged();
        emit winnerChanged();
    }
}
