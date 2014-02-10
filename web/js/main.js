// Chart definition
var chart;
var chart2;


// Once DOM (document) is finished loading
$(document).ready(function() {

    // First chart initialization
    var options ={
	chart: {
	    renderTo: 'chart',

	    events: {
		load : function() 
		{	
		    // set up the updating of the chart each second
		    var series = this.series[0];
		    setInterval(function() 
				{
				   
				    var date = chart.xAxis[0].getExtremes().dataMax;
				    $.getJSON('live-server-data.php','table=0&date='+date, 
					      function(data)
					      {
						  new_data = data;
					      });
				    while (new_data.length>0)
				    {
					punto = new_data.pop()
					chart.series[0].addPoint(punto, true, false);
				    }
				    }, 2000);
		}
	    }
	},
	title: {
	    text: 'Ejemplo'
	},
	rangeSelector: {
	    enabled: true
	},
	series: [{}],

	xAxis : {
	    type : 'datetime'
	},

    };

var options2 ={
	chart: {
	    renderTo: 'chart2',

	    events: {
		load : function() 
		{	
		    // set up the updating of the chart each second
		    var series = this.series[0];
		    setInterval(function() 
				{
				   
				    var date = chart.xAxis[0].getExtremes().dataMax;
				    $.getJSON('live-server-data.php','table=1&date='+date, 
					      function(data)
					      {
						  new_data = data;
					      });
				    while (new_data.length>0)
				    {
					punto = new_data.pop()
					chart.series[0].addPoint(punto, true, false);
				    }
				    }, 2000);
		}
	    }
	},
	title: {
	    text: 'Ejemplo'
	},
	rangeSelector: {
	    enabled: true
	},
	series: [{}],

	xAxis : {
	    type : 'datetime'
	},

    };

    $.getJSON('live-server-data.php','table=0&date=0', 
	      function(data)
	      {
		  options.series[0].data=data;
		  //chart= new Highcharts.Chart(options);
		  chart = new Highcharts.StockChart(options);
		  chart2 = new Highcharts.StockChart(options2);
	      });


    // Switchers (of the Chart1 type) - onclick handler
    $('.update').click(function () {
	var newType = $(this).attr('id');
    });
});


