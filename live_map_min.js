locs={};
companies=[];
companyObjs=[];
var markers = [];
companyCounter=0;
var points = [];
filterOverride="";
refreshSpeed=15000;

var greencirc="http://www.ijc.org/rel/ais-timeline/timeline/timeline_js/images/green-circle.png";
var redcirc="http://tng.lythgoes.net/wiki/images/a/a5/Red-circle.png";
var filter="";

var myLatlng = new google.maps.LatLng(39,-98);
var mapOptions = {
  zoom: 4,
  center: myLatlng
};

function formatTimestamp(timestamp) {
  var utcmilliseconds= timestamp;
  return formatMillisecondsTimestamp(utcmilliseconds);
}

function processHeartbeats(hbs) {
  $.each(hbs, function(key,val) {
    if ($.inArray("Location", val["payloadMap"]) &&
      undefined != val["payloadMap"]["Location"] &&
      $.inArray("payloadValues", val["payloadMap"]["Location"]) &&
      $.inArray("longitude", val["payloadMap"]["Location"]["payloadValues"])) {
        var company=val["payloadMap"]["PedalCoachInfo"]["payloadValues"]["company"];

        if ($.inArray(company, companies)==-1){
          companies.push(val["payloadMap"]["PedalCoachInfo"]["payloadValues"]["company"]);
        }


        timestamp=formatTimestamp([val["timestamp"]]);
        username=val["payloadMap"]["PedalCoachInfo"]["payloadValues"]["userName"];
        long=val["payloadMap"]["Location"]["payloadValues"]["longitude"];
        lat= val["payloadMap"]["Location"]["payloadValues"]["latitude"];
        var driveStatus = "Offline";
        try{
          driveStatus = val["payloadMap"]["ConnectionStatus"]["payloadValues"]["message"];
        }
        catch(e){
          console.log(e);
        }
        locs[username+"_"+timestamp]=long+","+lat+"_"+driveStatus;
      }
    });

    table="<table>";
    $.each(locs, function(key,val) {
      table+="<tr><td>"+key+"</td><td>"+val+"</td></tr>";
    });
    table+="</table>";

    $("#maps_map_container").html(table);

    companyCounter=companyCounter+1;
    if (companyObjs.length==companyCounter) {
      plot();
      buildDropDowns();
    }
  }

  function buildDropDowns() {
    titleBar=$("#summary_region");

    selectHTML='<select id="companySelect"><option value="">All</option>';

    $.each(companies, function (key,comp) {
      selectHTML=selectHTML+'<option value="' + comp + '">' + comp + '</option>';
    });

    selectHTML=selectHTML+'</select>';
    selectSpeedHTML='<select id="speedSelect"><option value="1000">1s</option><option value="10000">10s</option><option value="15000" selected=true>15s</option><option value="30000">30s</option></select>';

    titleBar.after(selectHTML).after(selectSpeedHTML);

    $("#companySelect").click(function() { filterOverride=$(this).val(); } );
    $("#speedSelect").click(function() { refreshSpeed=$(this).val(); });
  }

  function processCompanies(companies) {
    companyObjs=companies;
    $.each(companies, function (key,val) {
      $.ajax({
        url: "http://data.linkedrive.com/pdl/api/latestheartbeat?companyId="+key,
        headers: { "token":$.cookie("access_token") },
        success: function(result) { processHeartbeats(result); },
        error: function(result) { companyCounter=companyCounter+1; }
      });

    })
  }

  $.ajax({
    url: "http://data.linkedrive.com/pdl/api/company",
    headers: { "token":$.cookie("access_token") },
    success: function(result){ processCompanies(result); }
  });

  function reload(){
    $.ajax({
      url: "http://data.linkedrive.com/pdl/api/company",
      headers: { "token":$.cookie("access_token") },
      success: function(result){ processCompanies(result); }
    });
  };

  function formatMillisecondsTimestamp(utcmilliseconds){
    utcmilliseconds = parseInt(utcmilliseconds) / 1000;var d = new Date(0); d.setUTCSeconds(utcmilliseconds);return d;
  };
  vnes = document.getElementsByClassName("html-attribute-value");for (x=0; x<vnes.length;x++) {vne = vnes[x];date=(formatTimestamp(vne.innerHTML));vne.innerHTML=date;};

  function setAllMap(map) {
    for (var i = 0; i < markers.length; i++) {
      markers[i].setMap(map);
    }
    markers = [];
  }

  function plot(idx) {
    if (typeof(idx)=="undefined") {
      var idx=0;
    }
    var unitTransmitting = 0;

    filter=companies[idx];
    if (filterOverride != "") {
      filter=filterOverride;
      filterOveride = "";
    }

    $("#summary_region").html("<h3>"+filter+"</h3>");
    $("#left_side_bar").css("width","0px");
    $(".maps_wrapper").css("width","100%");
    $(".maps_wrapper").css("max-width","5000px");
    $("#gTable").css("visibility","hidden");
    $("#datepicker_container").css("visibility","hidden");
    var map = new google.maps.Map(document.getElementById("maps_map_container"), mapOptions);
    $.each(locs, function(key,val) {
      if (key.toLowerCase().indexOf(filter.toLowerCase()) == -1) {
        return true;
      }
      var lattemp = val.split(",")[1];
      var realLat = lattemp.split("_")[0]
      var driveStatus = lattemp.split("_")[1];

      var diff=Math.abs(new Date() - new Date(key.split("_")[1]));
      diff=diff/(1000*60*60);
      console.log(driveStatus);
      ll=val.split(",");
      if (diff < 5 && driveStatus == "Online."){
        img = greencirc;
        if ("null"!=ll[0] && "null"!=ll[1]){
          var marker = new google.maps.Marker({
            position: new google.maps.LatLng(realLat,val.split(",")[0] ),
            map: map,
            title: key.split("_")[0] + "\n" + key.split("_")[1],
            icon: img
          });
          points.push(marker);
          markers.push(marker);
        }
        unitTransmitting++;
      }
      else if (diff < 20) {
        img=redcirc;
        if ("null"!=ll[0] && "null"!=ll[1]){
          var marker = new google.maps.Marker({
            position: new google.maps.LatLng(realLat,val.split(",")[0] ),
            map: map,
            title: key.split("_")[0] + "\n" + key.split("_")[1],
            icon: img
          });
          points.push(marker);
          markers.push(marker);
        }
        unitTransmitting++;
      }
      else{
        unitTransmitting++;
      }

    });
    if(points.length > 0 )
    {
      var bounds = new google.maps.LatLngBounds();
      for(i=0;i<points.length;i++) {
        bounds.extend(points[i].getPosition());
      }

      map.fitBounds(bounds);
      if(map.getZoom()> 10){
        map.setZoom(10);
      }
    }
    else{
      console.log("no valid points for this company");
      if (idx >= companies.length || typeof(idx)=="undefined") {
        idx=0;
        plot (idx);
      }
      else{
        idx++;
        plot(idx);
      }

    }


    points = [];
    idx=idx+1;
    if (idx >= companies.length || typeof(idx)=="undefined") {
      idx=0;
      setAllMap(null)
      reload();
    }
    setTimeout(plot,refreshSpeed,idx);
  }
