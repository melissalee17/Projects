package solitaire;
import java.awt.Point;
import java.awt.Rectangle;
import javax.swing.ImageIcon;

public class Card {
    private String suit; 
    private ImageIcon card;
    private int number; //1-13, 1= ace, 2 = two, ... , 13 = king
    private int x, y;
    private boolean side, color, moving;
    final public static int CARD_HEIGHT = 117;
    final public static int CARD_WIDTH = 80;
    
    public Card(int num, String cardSuit, ImageIcon card_png)
    {
        number = num;
        suit = cardSuit;
        card = card_png;
        side = false;
        if (suit == "heart" || suit == "diamond")
            color = true; //red = ture
        else
            color = false; //black = false
        moving = false;
        
    }
    public Card (int num, ImageIcon card_png)
    {
        number = num;
        card = card_png;
        side = true;
    }
    public int getNumber()
    {
        return number;
    }
    public String getSuit()
    {
        return suit;
    }
    public boolean getSide()
    {
        return side;
    }
    public boolean getColor()
    {
        return color;
    }
    public void flipCard()
    {
        side = true;
    }
    public String toString()
    {
        String message = suit + " of " + number + "\nSide: " + side + "\n" + color;
        
        return message;
    }
    
    public void setPoint(Point p)
    {
        x = p.x;
        y = p.y;
    }
    public Point getPoint()
    {
        return new Point(x,y);
    }
    public ImageIcon getImage()
    {
        if (side == false)
            return new ImageIcon("back.png");
        return card;
    }
    public Rectangle getRectangle()
    {
        return new Rectangle(x, y, CARD_WIDTH, CARD_HEIGHT);
    }
    public boolean isMoving()
    {
        return moving;
    }
    public void setMoving(boolean move)
    {
        moving = move;
    }
}
