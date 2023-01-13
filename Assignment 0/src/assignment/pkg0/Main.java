package assignment.pkg0;
import java.util.Scanner;
import java.text.DecimalFormat;


 /**
 *
 * @author melissalee
 */
public class Main
{
   static DecimalFormat dft = new DecimalFormat("#.00");
   public static void main (String[] args)
   {
       // defines variables
       double radius, circ, area;
       //  call helper methods to input the radius
       radius = getRadius();
       //  call helper method to calc the 
       circ = calcCirc(radius);
       //  call helper method to calc th area
       area = calcArea(radius);
       // output the  calculated values
       System.out.println("Radius = " + dft.format(radius) + '\n' + "Circumference = " + dft.format(circ) + '\n' + "Area = " + dft.format(area));
   }
  
   public static double getRadius()
   {
        double radius;
        String temp;
        Scanner scan = new Scanner(System.in);
        System.out.println("Enter radius: ");
        temp = scan.nextLine();
        radius = Double.parseDouble(temp);
        return radius;
       
   }
 
   public static double calcCirc(double radius)
   {
       //calculate and return value for the circumference
       //using the radius passed as an argument
       return 2 * 3.14159 * radius;
   }
 
   public static double calcArea(double radius)
   {
       //calculate and return value for the area
       //using the radius passed as an argument
       return 3.14159 * radius * radius;
   }
}
 
