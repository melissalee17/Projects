package solitaire;

import java.util.Random;
import javax.swing.ImageIcon;


public class Deck {
    private Card[] deck;
    private Random rand;
    public Deck()
    {
        deck = new Card[52];
        rand = new Random();
        
        deck[0] = new Card(1, "spade", new ImageIcon("ace_of_spades.png"));
        deck[1] = new Card(2, "spade", new ImageIcon("2_of_spades.png"));
        deck[2] = new Card(3, "spade", new ImageIcon("3_of_spades.png"));
        deck[3] = new Card(4, "spade", new ImageIcon("4_of_spades.png"));
        deck[4] = new Card(5, "spade", new ImageIcon("5_of_spades.png"));
        deck[5] = new Card(6, "spade", new ImageIcon("6_of_spades.png"));
        deck[6] = new Card(7, "spade", new ImageIcon("7_of_spades.png"));
        deck[7] = new Card(8, "spade", new ImageIcon("8_of_spades.png"));
        deck[8] = new Card(9, "spade", new ImageIcon("9_of_spades.png"));
        deck[9] = new Card(10, "spade", new ImageIcon("10_of_spades.png"));
        deck[10] = new Card(11, "spade", new ImageIcon("jack_of_spades.png"));
        deck[11] = new Card(12, "spade", new ImageIcon("queen_of_spades.png"));
        deck[12] = new Card(13, "spade", new ImageIcon("king_of_spades.png"));
        
        deck[13] = new Card(1, "heart", new ImageIcon("ace_of_hearts.png"));
        deck[14] = new Card(2, "heart", new ImageIcon("2_of_hearts.png"));
        deck[15] = new Card(3, "heart", new ImageIcon("3_of_hearts.png"));
        deck[16] = new Card(4, "heart", new ImageIcon("4_of_hearts.png"));
        deck[17] = new Card(5, "heart", new ImageIcon("5_of_hearts.png"));
        deck[18] = new Card(6, "heart", new ImageIcon("6_of_hearts.png"));
        deck[19] = new Card(7, "heart", new ImageIcon("7_of_hearts.png"));
        deck[20] = new Card(8, "heart", new ImageIcon("8_of_hearts.png"));
        deck[21] = new Card(9, "heart", new ImageIcon("9_of_hearts.png"));
        deck[22] = new Card(10, "heart", new ImageIcon("10_of_hearts.png"));
        deck[23] = new Card(11, "heart", new ImageIcon("jack_of_hearts.png"));
        deck[24] = new Card(12, "heart", new ImageIcon("queen_of_hearts.png"));
        deck[25] = new Card(13, "heart", new ImageIcon("king_of_hearts.png"));
        
        deck[26] = new Card(1, "club", new ImageIcon("ace_of_clubs.png"));
        deck[27] = new Card(2, "club", new ImageIcon("2_of_clubs.png"));
        deck[28] = new Card(3, "club", new ImageIcon("3_of_clubs.png"));
        deck[29] = new Card(4, "club", new ImageIcon("4_of_clubs.png"));
        deck[30] = new Card(5, "club", new ImageIcon("5_of_clubs.png"));
        deck[31] = new Card(6, "club", new ImageIcon("6_of_clubs.png"));
        deck[32] = new Card(7, "club", new ImageIcon("7_of_clubs.png"));
        deck[33] = new Card(8, "club", new ImageIcon("8_of_clubs.png"));
        deck[34] = new Card(9, "club", new ImageIcon("9_of_clubs.png"));
        deck[35] = new Card(10, "club", new ImageIcon("10_of_clubs.png"));
        deck[36] = new Card(11, "club", new ImageIcon("jack_of_clubs.png"));
        deck[37] = new Card(12, "club", new ImageIcon("queen_of_clubs.png"));
        deck[38] = new Card(13, "club", new ImageIcon("king_of_clubs.png"));
        
        deck[39] = new Card(1, "diamond", new ImageIcon("ace_of_diamonds.png"));
        deck[40] = new Card(2, "diamond", new ImageIcon("2_of_diamonds.png"));
        deck[41] = new Card(3, "diamond", new ImageIcon("3_of_diamonds.png"));
        deck[42] = new Card(4, "diamond", new ImageIcon("4_of_diamonds.png"));
        deck[43] = new Card(5, "diamond", new ImageIcon("5_of_diamonds.png"));
        deck[44] = new Card(6, "diamond", new ImageIcon("6_of_diamonds.png"));
        deck[45] = new Card(7, "diamond", new ImageIcon("7_of_diamonds.png"));
        deck[46] = new Card(8, "diamond", new ImageIcon("8_of_diamonds.png"));
        deck[47] = new Card(9, "diamond", new ImageIcon("9_of_diamonds.png"));
        deck[48] = new Card(10, "diamond", new ImageIcon("10_of_diamonds.png"));
        deck[49] = new Card(11, "diamond", new ImageIcon("jack_of_diamonds.png"));
        deck[50] = new Card(12, "diamond", new ImageIcon("queen_of_diamonds.png"));
        deck[51] = new Card(13, "diamond", new ImageIcon("king_of_diamonds.png"));
    }
    
    public void shuffle()
    {
        for (int i = 0; i < deck.length; i++)
        {
            int num = rand.nextInt(52);
            Card temp = deck[i];
            deck[i] = deck[num];
            deck[num] = temp;
        }
    }
    
    public Card getCard(int i)
    {
        return deck[i];
    }
}
