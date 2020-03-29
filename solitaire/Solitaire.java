package solitaire;

import java.awt.*;
import java.util.Vector;
import javax.swing.*;

public class Solitaire {
    public static Deck deckOfCards = new Deck();
    public static Card[][] playCards = new Card[7][20];; //cards on playing field
    public static Vector<Card> pile = new Vector(); //cards that are to be flipped over
    public static Vector<Card> heap = new Vector(); //cards that have been flipped over
    public static Card[] topCards = new Card[7];; //top cards that are on the playing field
    public static Card[][] scoring = new Card[4][13]; //cards in scoring pile
    public static Card[] scoringTopCards = new Card[4]; //top cards that are in scoring pile
    public static Boolean goodLocation = false; //if card is clicked on, returns
    //true if there is place on the playing field it could go 
    //false if there not a place on the playing field it could go
    public static Card empty = new Card(0, new ImageIcon("empty.png")); //when there is nothing at bottom of playing stack
    public static Card empty1 = new Card(0, new ImageIcon("winstack.png")); //when there is nothing on scoring stacks
    public static Card moveCard;
    public static void main(String[] args) {
        deal();
        JFrame frame = new JFrame("Solitaire");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(new SolitairePanel());
        frame.pack();
        frame.setVisible(true);
    }
    
    public static void deal()
    {
        deckOfCards.shuffle();
        int counter = 0;
        //28 cards in playing field
        for (int j = 0; j < 7; j++)
        {
            for (int i = j; i < 7; i++)
            {
                playCards[i][j] = deckOfCards.getCard(counter);
                if (i == j)
                {
                    playCards[i][j].flipCard();
                    topCards[i] = playCards[i][j];
                }
                counter++;
            }
        }
        //24 cards pile
        for (int i = 28; i < 52; i++)
        {
            pile.add(deckOfCards.getCard(i));
        }
        for (int i = 0; i < 4; i++)
        {
            scoring[i][0] = empty1;
            scoringTopCards[i] = scoring[i][0];
        }
    }
    public static Card getPlayCard(int i, int j)
    {
        return playCards[i][j];
    }
    public static void nextCard()
    {
        if (!pile.isEmpty()){
            heap.addElement(pile.firstElement());
            pile.remove(0);
            play(1);
        }
        else{
            pile.addAll(heap);
            heap.removeAllElements();
        }
    }
    public static boolean goodMove(Card card)
    {
        goodLocation = false;
        for (int i = 0; i < 7; i++)
        {
            //if open space
            if (topCards[i].getNumber() == 0)
            {
                //if card is a king
                if (card.getNumber() == 13){
                    goodLocation = true;
                }
            }
            //if top card is opposite color suit and 1 value higher
            else if (card.getColor() != topCards[i].getColor() && card.getNumber() + 1 == topCards[i].getNumber() && topCards[i].getNumber() != 0)
                goodLocation = true;
        }
        return goodLocation;
    }
    public static boolean MoveToScore(Card card)
    {
        goodLocation = false;
        for (int i = 0; i < 4; i++)
        {
            //if open space and card is an ace
            if (scoringTopCards[i].getNumber() == 0 && card.getNumber() == 1){
                goodLocation = true;
                break;
            }
            //next of same of same suit
            if (card.getSuit().equals(scoringTopCards[i].getSuit()) && card.getNumber() - 1 == scoringTopCards[i].getNumber() && scoringTopCards[i].getNumber() != 0)
                goodLocation = true;
        }
        return goodLocation;
    }
    public static void HeapToField()
    {
        Card card = heap.lastElement();
        //if card can be scored
        if (MoveToScore(heap.lastElement()))
        {
            //go through scoring piles
            for (int i = 0; i < 4; i++)
            {
                //if pile is empty && card is an ace
                if (scoring[i][0].getNumber() == 0 && card.getNumber() == 1)
                    {
                        scoring[i][0] = card;
                        scoringTopCards[i] = card;
                        moveCard = card;
                        heap.remove(card);
                        play(1);
                        break;
                    }
                for (int j = 0; j < 13; j++)
                {
                    if (scoring[i][j] == null)
                        continue;
                    //if top card
                    else if (scoring[i][j] == scoringTopCards[i] && scoring[i][j].getNumber() != 0)
                    {
                        //if next card with same suit
                        if (scoring[i][j].getSuit().equals(card.getSuit()) && card.getNumber() - 1 == scoring[i][j].getNumber())
                        {
                            scoring[i][j + 1] = card;
                            scoringTopCards[i] = card;
                            moveCard = card;
                            heap.remove(card);
                            play(1);
                            break;
                        }
                    }
                }
            }
        }
        //if can be placed on playing field
        else if (goodMove(heap.lastElement()))
        {
            //go through playing cards
            for (int i = 0; i < 7; i++)
            {
                //if open space and card is king
                if (playCards[i][0].getNumber() == 0 && card.getNumber() == 13)
                    {
                        //place king
                        playCards[i][0] = card;
                        topCards[i] = card;
                        moveCard = card;
                        heap.remove(card);
                        play(1);
                        break;
                    }
                for (int j = 0; j < 20; j++)
                {
                    //if top card
                    if (playCards[i][j] == topCards[i] && playCards[i][j].getNumber() != 0)
                    {
                        //if card can be placed on playing field (different color and 1 less)
                        if (card.getColor() != playCards[i][j].getColor() && card.getNumber() + 1 == playCards[i][j].getNumber()){
                            playCards[i][j + 1] = card;
                            topCards[i] = card;
                            moveCard = card;
                            heap.remove(card);
                            play(1);
                            return;
                        }
                    }
                }
            }
        }
    }
    public static void moveStack(Vector<Card> transferStack)
    {
        Card card = transferStack.firstElement();
        //if can be scored
        if (transferStack.size() == 1 && MoveToScore(card))
        {
            for (int i = 0; i < 4; i++)
            {
                //if pile is empty && card is an ace
                if (scoring[i][0].getNumber() == 0 && card.getNumber() == 1)
                    {
                        scoring[i][0] = card;
                        scoringTopCards[i] = card;
                        transferStack.remove(card);
                        play(1);
                        break;
                    }
                for (int j = 0; j < 13; j++)
                {
                    if (scoring[i][j] == scoringTopCards[i] && scoring[i][j].getNumber() != 0)
                    {
                        //if next card with same suit
                        if (scoring[i][j].getSuit().equals(card.getSuit()) && card.getNumber() - 1 == scoring[i][j].getNumber())
                        {
                            scoring[i][j + 1] = card;
                            scoringTopCards[i] = card;
                            transferStack.remove(card);
                            play(1);
                            break;
                        }
                    }
                }
            }
        }
        //can be placed somewhere on playing field
        else if (goodMove(card))
        {
            for (int i = 0; i < 7; i++)
            {
                //if king
                if (playCards[i][0] == empty && card.getNumber() == 13)
                    {
                        int counter = 0;
                        while (!transferStack.isEmpty())
                        {
                            card = transferStack.firstElement();
                            playCards[i][counter] = card;
                            topCards[i] = card;
                            transferStack.remove(card);
                            counter++;
                        }
                        play(1);
                        break;
                    }
                for (int j = 0; j < 20; j++)
                {
                    //if top card
                    if (playCards[i][j] == topCards[i] && playCards[i][j].getNumber() != 0)
                    {
                        //if can be placed on playing field (different color)
                        if (card.getColor() != playCards[i][j].getColor() && card.getNumber() + 1 == playCards[i][j].getNumber()){
                            int counter = j + 1;
                            while (!transferStack.isEmpty())
                            {
                                card = transferStack.firstElement();
                                playCards[i][counter] = card;
                                topCards[i] = card;
                                transferStack.remove(card);
                                counter++;
                            }
                            play(1);
                            return;
                        }
                    }
                }
            }
        }
        
    }
    public static void play (int sound)
    {
        try {
            switch (sound)
            {
                case 1:
                    PlaySound.myPlay("file:/Users/melissalee/Desktop/goodClick.wav");
                    break;
                case 2:
                    PlaySound.myPlay("file:/Users/melissalee/Desktop/badClick.wav");
                    break;
                case 3:
                    PlaySound.myPlay("file:/Users/melissalee/Desktop/cheer.wav");
                    break;
                default:
                    break;
            }
        }
        catch (InterruptedException ie)
        {
            System.out.println(ie);
        }
    }
    public static boolean winGame()
    {
        if (heap.isEmpty() && pile.isEmpty())
        {
            for (int i = 0; i < 7; i++)
            {
                if (topCards[i] != empty)
                {
                    return false;
                }
            }
            play(3);
            return true;
        }
        else
            return false;
    }
}
