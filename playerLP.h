#ifndef PLAYER_H 
#define PLAYER_H

#include<algorithm>
#include<random>
#include<string>
#include "board.h"
#include "move.h"
#include "game.h"

/** In questo namespace potete aggiungere tutte le classi e le funzioni di 
 *  cui avete bisogno, ma non potete in alcun modo modificare 
 *  le funzioni già esistenti.
 */
namespace player {
    // NON TOCCARE. Classe astratta, utile per i test.
    class AbstractPlayer {
    public:
        virtual std::string getName() = 0;
        virtual std::string getIDs() {
            return "";
        }
        virtual move::Move getMoveFromSit(game::Situation situation) = 0;
    };

    /**
     * CLASSE DA COMPLETARE PER IL PROGETTO.
     * Classe che implementa il giocatore di Hex.
     * Il giocatore è caratterizzato da un nome, dai
     * numeri di matricola degli studenti che lo hanno
     * programmato e dalla funzione getMoveFromSit(Situation sit)
     * che riceve una situazione di gioco e ritorna in output
     * la mossa scelta dal giocatore in tale situazione.
     */
    class Player : public AbstractPlayer {
        public:
        /**
         * @brief Restituisce  il nome della squadra
         * INSERIRE IL NOME!
         * @return std::string nome della squadra
         */
        std::string getName() {
            std::string player_name="Percival";
            return player_name;
        }

        /**
         * @brief restituisce le matricole dei due componenti della squadra
         * INSERIRE LE MATRICOLE!
         * @return std::string matricole separate da uno spazio
         */
        std::string getIDs() {
            std::string matricola_1="1966415";
            std::string matricola_2="1966420";
            return matricola_1 + " " + matricola_2;
        }


		int simulation(board::HexBoard board,int my_turn,std::vector<std::pair<int,int>> empty_pieces,int seme) {
            int turno=my_turn;
            std::shuffle(empty_pieces.begin(), empty_pieces.end(), std::default_random_engine(seme));
            for(std::pair<int,int> pos : empty_pieces) {
                if(turno==1) 
                    board.addPiece(board::Piece::BLUE_DISC,pos);
                if(turno==2)
                    board.addPiece(board::Piece::RED_DISC,pos);
                turno=3-turno;
            }
            if(board.checkWin(my_turn))
                return 1;
            else
                return -1;
        }



        int MCTS(game::Situation situation, int my_turn) {
            std::vector<std::vector<board::Piece>> board_view = situation.getBoardView();
            board::HexBoard board(board_view);
            std::vector<std::pair<int,int>> empty_pieces=board.getPosByPiece(board::Piece::EMPTY);
            int your_turn;
            your_turn=3-my_turn;
            if(board.checkWin(my_turn)) 
                return 1000;
            if(board.checkWin(your_turn)) 
                return -1000;
            int score=0;
            for(int i=1;i<=40;i++) {
                score=score+simulation(board,my_turn,empty_pieces,i);
                board=board_view;
            }    
            return score;    
        }




        /**
         * FUNZIONE DA IMPLEMENTARE PER IL PROGETTO. NON cambiare
         * l'intestazione, ma scrivere solo nel corpo della funzione.
         * 
         * Questa funzione riceve in input una configurazione o situazione
         * di gioco e restituisce in output la mossa scelta dal giocatore.
         * 
         * @param situation configurazione o situazione di gioco
         * @return move::Move mossa scelta dal giocatore
         */
        move::Move getMoveFromSit(game::Situation situation) {
            move::Move m; // Si noti che una mossa inizializzata in questo modo è di tipo RESIGN (mossa vuota)
            std::vector<move::Move> mosse_valide = situation.validMoves();
            game::Situation new_situation;
            int my_turn=situation.turn;
            int migliore_mossa=-2000;
            int valore_mossa;
            for(move::Move& x : mosse_valide) {
                new_situation = situation.next(x);
                valore_mossa=MCTS(new_situation,my_turn);
                if(valore_mossa==1000)
                    return x;
                if(valore_mossa>migliore_mossa) {
                    m=x;
                    migliore_mossa=valore_mossa;
                }
            }
            return m;
        }
    };

}

#endif