#pragma once


#include <QObject>
#include <QVector>
#include <QVariant>
#include <QVariantList>



class TicTacToeGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList board READ board NOTIFY boardChanged)
    Q_PROPERTY(int currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
    Q_PROPERTY(int winner READ winner NOTIFY winnerChanged)


public:
    explicit TicTacToeGame(QObject *parent = nullptr);


    QVariantList board() const;
    int currentPlayer() const; // 1 = X, -1 = O
    bool gameOver() const;
    int winner() const; // 0 = none/draw, 1 = X, -1 = O


    Q_INVOKABLE bool makeMove(int index);
    Q_INVOKABLE void reset();


signals:
    void boardChanged();
    void currentPlayerChanged();
    void gameOverChanged();
    void winnerChanged();


private:
    QVector<int> m_board; // 0 empty, 1 X, -1 O
    int m_currentPlayer;
    bool m_gameOver;
    int m_winner;


    void checkGameState();
};
