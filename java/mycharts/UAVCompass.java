

/*
 * 
 * remove clickable button and field at bottom of chart when done so
 *
*/



package mycharts;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.Toolkit;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.CompassPlot;
import org.jfree.data.general.DefaultValueDataset;
import org.jfree.ui.Spinner;
import org.jfree.chart.plot.dial.*;
import org.jfree.chart.plot.dial.DialPointer.*;
import org.jfree.chart.plot.dial.StandardDialFrame;
import org.jfree.ui.GradientPaintTransformType;
import org.jfree.ui.StandardGradientPaintTransformer;



public class UAVCompass extends JPanel 
{

	// initialize components
    	private DefaultValueDataset compassData = new DefaultValueDataset(new Double(0.0));
    	private CompassPlot compassPlot = new CompassPlot(this.compassData);
    	private JFreeChart compassChart = new JFreeChart("Compass Test", JFreeChart.DEFAULT_TITLE_FONT, this.compassPlot, false);
    	private ChartPanel compassChartPanel = new ChartPanel(this.compassChart);
    	private JPanel compassHolderPanel = new JPanel();
    	private JPanel compassDialPanel = new JPanel();
    	private BorderLayout borderLayout = new BorderLayout();
    	private GridBagLayout gridBagLayout = new GridBagLayout();
    	private GridLayout compassDialPanelGridLayout = new GridLayout();
    	private GridLayout compassGridLayout = new GridLayout();
    	private Spinner compassDial = new Spinner(45);



    	// compass constructor
    	public UAVCompass() 
	{
        	try 
		{
            		this.compassPlot.addDataset(this.compassData);
            		this.compassPlot.setSeriesPaint(1, Color.red);
            		this.compassPlot.setSeriesOutlinePaint(1, Color.red);
            		compassInit();
        	}
        	catch (Exception ex) 
		{
	            	ex.printStackTrace();
        	}
    	}



    	/*
     	 * initialises the compass
     	 *
     	 * @throws Exception if there are any exceptions.
     	 */
    	private void compassInit() throws Exception 
	{

       		setLayout(this.compassGridLayout);
       		this.compassHolderPanel.setLayout(this.borderLayout);


		this.compassDial.setEnabled(true);
        	this.compassDial.addPropertyChangeListener(new PropertyChangeListener() 
		{
        		public void propertyChange(final PropertyChangeEvent evt) 
			{
        			final Spinner spinner = (Spinner) evt.getSource();
        			if (spinner.isEnabled()) 
				{
        	        		compassData.setValue(new Double(spinner.getValue()));
        	      		}
			}
		});

	
		this.compassDialPanel.setLayout(this.compassDialPanelGridLayout);
        	this.compassDialPanel.setLayout(this.gridBagLayout);


        	add(this.compassHolderPanel, null);
        	this.compassHolderPanel.add(this.compassDialPanel, BorderLayout.SOUTH);

        	this.compassDialPanel.add(this.compassDial,
                	new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
                	GridBagConstraints.CENTER,
                	GridBagConstraints.BOTH,
                	new Insets(0, 0, 0, 0),
                	0, 0));

        	this.compassHolderPanel.add(this.compassChartPanel, BorderLayout.CENTER);
    	}


    	/*
     	 * pdates the compass' data
     	 *
     	 * @param value  the value.
     	 */
    	public void adjustData(final double value) 
	{
	        final Number val = this.compassData.getValue();
	        double newVal = value;
	        if (val != null) 
		{
	        	newVal += val.doubleValue();
        	}
        	if (newVal > 360) {
          		newVal = 0;
        	}
        	if (newVal < 0) {
            		newVal = 360;
        	}

        	this.compassData.setValue(new Double(newVal));
    	}


}
