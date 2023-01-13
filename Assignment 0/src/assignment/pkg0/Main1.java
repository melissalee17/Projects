/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package assignment.pkg0;

//=========== Code to use with Version 2 (two .java files)==================
//    ----- Main.java-------

import java.text.DecimalFormat;
import java.util.Scanner;

public class Main1
{
    static DecimalFormat dft = new DecimalFormat("#.00");
   public static void main (String[] args)
   {
       //  defines Circle object;
       //  call helper(class) method to input the radius
       //  instantiate the Circle object;
       Circle NewCircle = new Circle(getRadius());
       //  output the circumference using instance method
       System.out.println("Radius = " + dft.format(NewCircle.getRadius()) + '\n' + "Circumference = " + dft.format(NewCircle.getCircumference()) + '\n' + "Area = " + dft.format(NewCircle.getArea()));
       //  output the area using instance method
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
}
