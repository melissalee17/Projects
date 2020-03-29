package solitaire;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;

public class SolitairePanel extends JPanel
{
    private final int WIDTH = 660, HEIGHT = 500;
    private int x, y, moveX = 10, moveY = 10;
    private final Rectangle stack1 = new Rectangle(20, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT); 
    private final Rectangle stack2 = new Rectangle (110, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle winStack1 = new Rectangle(290, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT); 
    private final Rectangle winStack2 = new Rectangle(380, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle winStack3 = new Rectangle(470, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle winStack4 = new Rectangle(560, 10, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle playStack1 = new Rectangle(20, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT); 
    private final Rectangle playStack2 = new Rectangle(110, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle playStack3 = new Rectangle(200, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT); 
    private final Rectangle playStack4 = new Rectangle(290, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle playStack5 = new Rectangle(380, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle playStack6 = new Rectangle(470, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private final Rectangle playStack7 = new Rectangle(560, 150, Card.CARD_WIDTH, Card.CARD_HEIGHT);
    private static Card card;
    private static ImageIcon image, image1 = new ImageIcon("winstack.png");
    private static Point point;
    private static Vector<Card> transferStack = new Vector();
    
    public SolitairePanel()
    {
        SolitaireListener listener = new SolitaireListener();
        addMouseListener(listener);
        setBackground(Color.GREEN);
        setPreferredSize(new Dimension(WIDTH, HEIGHT));
    }
    //have paint component
    public void paintComponent(Graphics page)
    {
        super.paintComponent(page);
        image = new ImageIcon("back.png");
        image.paintIcon(this, page, stack1.x, stack1.y);
        if (!Solitaire.heap.isEmpty())
        {
            int size = Solitaire.heap.size();
            card = Solitaire.heap.elementAt(size - 1);
            card.flipCard();
            card.setPoint(stack2.getLocation());
            image = card.getImage();
            image.paintIcon(this, page, stack2.x, stack2.y);
        }
        //add code for cards on winstacks
        image = Solitaire.scoringTopCards[0].getImage();
        image.paintIcon(this, page, winStack1.x, winStack1.y);
        image = Solitaire.scoringTopCards[1].getImage();
        image.paintIcon(this, page, winStack2.x, winStack2.y);
        image = Solitaire.scoringTopCards[2].getImage();
        image.paintIcon(this, page, winStack3.x, winStack3.y);
        image = Solitaire.scoringTopCards[3].getImage();
        image.paintIcon(this, page, winStack4.x, winStack4.y);
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                if (Solitaire.playCards[i][j] != null){
                    switch (i)
                    {
                        case 0:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack1.x, playStack1.y + 10 * j));
                            break;
                        case 1:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack2.x, playStack2.y + 10 * j));
                            break;
                        case 2:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack3.x, playStack3.y + 10 * j));
                            break;
                        case 3:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack4.x, playStack4.y + 10 * j));
                            break;
                        case 4:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack5.x, playStack5.y + 10 * j));
                            break;
                        case 5:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack6.x, playStack6.y + 10 * j));
                            break;
                        case 6:
                            Solitaire.playCards[i][j].setPoint(new Point(playStack7.x, playStack7.y + 10 * j));
                            break;
                        default:
                            break;
                    }
                    card = Solitaire.playCards[i][j];
                    image = card.getImage();
                    image.paintIcon(this, page, card.getPoint().x, card.getPoint().y);
                    
                }
            }
        }
    }
    //have action listener
    private class SolitaireListener implements MouseListener
    {
        public void mousePressed (MouseEvent event)
        {
            point = event.getPoint();
            if (stack1.contains(point))
            {
                Solitaire.nextCard();
                repaint();
                //play success sound
            }
            if (stack2.contains(point))
            {
                Solitaire.HeapToField();
                repaint();
            }
            else {
                transferStack.clear();
                boolean flag = false; //if the card has been found flag = true
                for (int i = 0; i < 7; i++)
                {
                    for (int j = 0; j < 20; j++)
                    {
                        if (Solitaire.playCards[i][j] == null)
                            continue;
                        //if card is clicked on 
                        if (Solitaire.playCards[i][j].getRectangle().contains(point) && !flag && Solitaire.playCards[i][j].getSide())
                        {
                            
                            //if can be played on field or can be scored
                            if (Solitaire.goodMove(Solitaire.playCards[i][j]) || Solitaire.MoveToScore(Solitaire.playCards[i][j]))
                            {
                                //add card to transfer stack and set it's position to null
                                transferStack.add(Solitaire.playCards[i][j]);
                                Solitaire.playCards[i][j] = null;
                            }
                            else{ 
                                Solitaire.play(2);
                                return;
                            }
                            //if cards underneath
                            if (j != 0 && !Solitaire.playCards[i][j - 1].getSide()){
                                //flip card underneath
                                Solitaire.playCards[i][j - 1].flipCard();
                                //make card underneath the top card
                                Solitaire.topCards[i] = Solitaire.playCards[i][j - 1];
                            }
                            //if no cards underneath
                            if (j == 0)
                            {
                                //set top card of stack empty
                                Solitaire.playCards[i][j] = Solitaire.empty;
                                Solitaire.topCards[i] = Solitaire.empty;
                            }
                            flag = true; //card has been found
                        }
                        //if there are cards below clicked on card
                        else if (flag)
                        {
                            //add card to transfer stack
                            transferStack.add(Solitaire.playCards[i][j]);
                            //set cards that being transfered to null
                            Solitaire.playCards[i][j] = null;
                        }
                    }
                    if (flag)
                        break;
                }
                //if card was found and stack isn't empty
                if (!transferStack.isEmpty()){
                    //check to see if stack can be transfered to another stack
                    Solitaire.moveStack(transferStack);
                    repaint();
                }
                
            }
        }
        public void mouseReleased (MouseEvent event) {}
        public void mouseClicked(MouseEvent event) {}
        public void mouseEntered(MouseEvent e) {}

        public void mouseExited(MouseEvent e) {}
    }
}
