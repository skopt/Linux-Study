package com.wenix;  
  
import java.awt.Dimension;  
import java.awt.Graphics;  
import java.awt.Image;  
import java.awt.Toolkit;  
import java.awt.event.ActionEvent;  
import java.awt.event.ActionListener;  
import java.io.File;  
import java.io.FileNotFoundException;  
import java.io.IOException;  
import java.io.InputStream;  
import java.io.RandomAccessFile;  
import java.net.ServerSocket;  
import java.net.Socket;  
  
import javax.imageio.ImageIO;  
import javax.swing.JButton;  
import javax.swing.JFileChooser;  
import javax.swing.JFrame;  
import javax.swing.JPanel;  
  
public class ImageServer {          
    public static ServerSocket ss = null;  
      
    public static void main(String args[]) throws IOException{      
            ss = new ServerSocket(6000);  
          
        final ImageFrame frame = new ImageFrame(ss);  
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        frame.setVisible(true);  
         
        while(true){  
                frame.panel.getimage();  
            frame.repaint();  
        }          
    }  
         
}  
  
/**  
    A frame with an image panel 
*/  
@SuppressWarnings("serial")  
class ImageFrame extends JFrame{  
        public ImagePanel panel;  
        public JButton jb;  
     
    public ImageFrame(ServerSocket ss){  
        // get screen dimensions                
        Toolkit kit = Toolkit.getDefaultToolkit();  
        Dimension screenSize = kit.getScreenSize();  
        int screenHeight = screenSize.height;  
        int screenWidth = screenSize.width;  
  
        // center frame in screen  
        setTitle("ImageTest");  
        setLocation((screenWidth - DEFAULT_WIDTH) / 2, (screenHeight - DEFAULT_HEIGHT) / 2);  
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);  
  
        // add panel to frame  
        this.getContentPane().setLayout(null);  
        panel = new ImagePanel(ss);  
        panel.setSize(640,480);  
        panel.setLocation(0, 0);  
        add(panel);  
        jb = new JButton("Take Photo");  
        jb.setBounds(0,480,640,50);  
        add(jb);  
        saveimage saveaction = new saveimage(ss);  
        jb.addActionListener(saveaction);  
    }  
  
    public static final int DEFAULT_WIDTH = 640;  
    public static final int DEFAULT_HEIGHT = 560;    
}  
  
/** 
   A panel that displays a tiled image 
*/  
@SuppressWarnings("serial")  
class ImagePanel extends JPanel {       
    private ServerSocket ss;  
    private Image image;  
    private InputStream ins;  
           
    public ImagePanel(ServerSocket ss) {    
            this.ss = ss;  
    }  
      
    public void getimage() throws IOException{  
            Socket s = this.ss.accept();  
        System.out.println("Conneted");  
        this.ins = s.getInputStream();  
                this.image = ImageIO.read(ins);  
                this.ins.close();  
    }  
     
    public void paintComponent(Graphics g){    
        super.paintComponent(g);      
        if (image == null) return;  
        g.drawImage(image, 0, 0, null);  
    }  
  
}  
  
class saveimage implements ActionListener {  
        RandomAccessFile inFile = null;  
        byte byteBuffer[] = new byte[1024];  
        InputStream ins;  
        private ServerSocket ss;  
          
        public saveimage(ServerSocket ss){  
                this.ss = ss;  
        }  
          
        public void actionPerformed(ActionEvent event){  
        try {  
                Socket s = ss.accept();  
                ins = s.getInputStream();  
                          
                JFileChooser jfc = new JFileChooser(".");  
                jfc.showSaveDialog(new javax.swing.JFrame());  
                File savedFile = jfc.getSelectedFile();  
                  
                if (savedFile != null) {  
                    try {  
	                        inFile = new RandomAccessFile(savedFile, "rw");  
	                } catch (FileNotFoundException e) {  
	                        e.printStackTrace();  
	                }  
                }  
  
            int amount;  
            while ((amount = ins.read(byteBuffer)) != -1) {  
                inFile.write(byteBuffer, 0, amount);  
            }  
            inFile.close();  
            ins.close();  
            s.close();  
            javax.swing.JOptionPane.showMessageDialog(new javax.swing.JFrame(),  
                    "Saved", "Information", javax.swing.JOptionPane.PLAIN_MESSAGE);  
                } catch (IOException e) {  
  
                        e.printStackTrace();  
                }  
        }  
}  

