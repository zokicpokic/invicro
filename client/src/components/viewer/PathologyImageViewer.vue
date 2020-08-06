<template>
  <div>
    <div id="mapOL"></div>
    <form class="form-inline">
      <label>Geometry type &nbsp;</label>
      <select id="type">
        <option value="Polygon">Polygon</option>
      </select>
      <br />
      <br />
      <label for="displayFeatureColor">Feature Color</label>
      <input type="color" id="displayFeatureColor" @change="featureColorChange($event)" />
      <br />
      <br />
      <input type="checkbox" id="displayMLDId" @change="check($event)" checked />
      <label for="displayMLDId">Display MLD Overlay</label>
    </form>
  </div>
</template>

<script>
import axios from "axios";
import constants from "@/utils/constants.js";
import Map from "ol/Map.js";
import View from "ol/View.js";
import XYZ from "ol/source/XYZ";
import { Draw, Modify, Snap } from "ol/interaction";
import { Tile as TileLayer, Vector as VectorLayer } from "ol/layer";
import { Vector as VectorSource } from "ol/source";
import GeoJSON from "ol/format/GeoJSON";
import { Circle as CircleStyle, Fill, Stroke, Style } from "ol/style";
import { bbox } from "ol/loadingstrategy";
import { mapGetters } from "vuex";
import * as m from "../../store/mutation_types";
import * as a from "../../store/action_types";

//To DO:
//implement vie store
//include Image width/height as state params
export default {
  name: "pathology-image-viewer",
  data: function () {
    return {
      dataMap: null,
      featureColor: "#00000",
    };
  },
  computed: {
    ...mapGetters([
      "imageWidth",
      "imageHeight",
      "annotation",
      "activeProjectId",
      "activeStudyId",
      "activeSerieId",
      "annotation"
    ]),
  },
  watch: {
    $route: function () {
      console.log("Route changed");
      this.loadProject();
    },
  },
  mounted: function () {
    this.loadProject();
  },
  methods: {
    check: function (e) {
      var layer = this.dataMap?.getLayers().array_[1];
      layer?.setVisible(e.target.checked);
    },
    featureColorChange(e) {
      this.featureColor = e.target.value;
    },
    loadProject: function () {
      const projectId = this.$route.params.projectId;
      const serieId = this.$route.params.serieId;

      this.$store.commit(m.PROJECTS_SET_ACTIVE, {
        projectId: projectId,
        studyId: 1,
        serieId: serieId,
      });
      var f = this.$store.dispatch(a.PROJECTS_FETCH_DIMENSIONS);
      var f2 = this.$store.dispatch(a.PROJECTS_FETCH_ANNOTATION);

      Promise.all([f, f2])
        .then(() => {})
        .catch((e) => {
          console.log("error fetcing data");
          console.log(e);
        });

      var self = this;
      //TODO: studyId nije 1?
      const mldId =
        self.$route.params.serieId.substring(
          0,
          self.$route.params.serieId.length - 4
        ) + ".mld";

      const serId = self.$route.params.serieId;
      var imgWidth, imgHeight;
      axios
        .get(constants.API_CORE_HOST + "/api/mld/dimensions/1/1/" + serId + "/")
        .then((response) => {
          console.log(response);
          imgWidth = response.data.width;
          imgHeight = response.data.height;
          // calculation max extension
          var maxZoomLevel =
            Math.floor(Math.max(imgWidth - 1, imgHeight - 1) / 256) + 1;
          maxZoomLevel = Math.ceil(Math.log(maxZoomLevel) / Math.log(2)) + 1;
          var maxResolution = Math.pow(2, maxZoomLevel - 1);
          var MAX_EXTENTION = maxResolution * 256;

          //calculate restricted extent
          var extent = [
            -20026376.39,
            -20048966.1 +
              ((2 * (MAX_EXTENTION - imgHeight)) / MAX_EXTENTION) * 20048966.1,
            -20026376.39 + ((2 * imgWidth) / MAX_EXTENTION) * 20026376.39,
            20048966.1,
          ];
          var view = new View({
            center: [
              (((2 * imgWidth) / MAX_EXTENTION) * 20026376.39) / 2,
              (((2 * (MAX_EXTENTION - imgHeight)) / MAX_EXTENTION) *
                20048966.1) /
                2,
            ],
            zoom: maxZoomLevel / 2,
            maxZoom: maxZoomLevel - 1,
            extent: extent,
          });

          //use tiles on the fly
          //to do add bbox to state and return collection of tiles
          var baseLayer = new TileLayer({
            name: "Main",
            source: new XYZ({
              url:
                constants.API_CORE_HOST +
                "/api/region/" +
                serId +
                "/{z}/{x}/{y}",
              wrapX: false,
            }),
          });

          var source = new VectorSource({
            format: new GeoJSON(),
            wrapX: false,
            loader: function () {
              var url =
                constants.API_CORE_HOST +
                "/api/mld/1/1/" +
                mldId +
                "/" +
                serId +
                "/";
              axios.get(url).then((response) => {
                if (response.status == 200) {
                  var res = response.data;
                  if (res.features) {
                    var maxZoomLevel =
                      Math.floor(Math.max(imgWidth - 1, imgHeight - 1) / 256) +
                      1;
                    maxZoomLevel =
                      Math.ceil(Math.log(maxZoomLevel) / Math.log(2)) + 1;
                    var maxResolution = Math.pow(2, maxZoomLevel - 1);
                    var maxExtention = maxResolution * 256;

                    for (var i = 0; i < res.features.length; i++) {
                      var coordinates = res.features[i].geometry.coordinates;
                      if (coordinates) {
                        if (res.features[i].geometry.type != "Point") {
                          for (var j = 0; j < coordinates.length; j++) {
                            var coordinates2 = coordinates[j];
                            if (coordinates2) {
                              for (var k = 0; k < coordinates2.length; k++) {
                                var x =
                                  res.features[i].geometry.coordinates[j][
                                    k
                                  ][0] *
                                    1000 +
                                  imgWidth / 2;
                                x =
                                  (x * 40052752.78) / maxExtention -
                                  20026376.39;
                                var y =
                                  imgHeight / 2 -
                                  res.features[i].geometry.coordinates[j][
                                    k
                                  ][1] *
                                    1000;
                                y =
                                  ((maxExtention - y) * 40097932.2) /
                                    maxExtention -
                                  20048966.1;
                                res.features[i].geometry.coordinates[j][
                                  k
                                ][0] = x;
                                res.features[i].geometry.coordinates[j][
                                  k
                                ][1] = y;
                              }
                            }
                          }
                        } else {
                          var x1 = res.features[i].geometry.coordinates[0];
                          x1 = (x1 * 40075016.68) / maxExtention - 20037508.34;
                          var y1 = res.features[i].geometry.coordinates[1];
                          y1 =
                            ((maxExtention - y1) * 40075016.68) / maxExtention -
                            20037508.34;
                          res.features[i].geometry.coordinates[0] = x1;
                          res.features[i].geometry.coordinates[1] = y1;
                        }
                      }
                    }
                  }
                  source.clear();
                  source.addFeatures(
                    source.getFormat().readFeatures(JSON.stringify(res))
                  );
                }
              });
            },
            strategy: bbox,
          });

          var vector = new VectorLayer({
            name: "Vector",
            source: source,
            style: new Style({
              fill: new Fill({
                color: "rgba(255, 255, 255, 0.2)",
              }),
              stroke: new Stroke({
                color: "#ffcc33",
                width: 2,
              }),
              image: new CircleStyle({
                radius: 2,
                fill: new Fill({
                  color: "#ffcc33",
                }),
              }),
            }),
          });

          var sourceAnnotations = new VectorSource({
            format: new GeoJSON(),
            wrapX: false,
            loader: function () {
              var url =
                constants.API_CORE_HOST + "/api/annotations/1/1/" + serId + "/";
              axios.get(url).then((response) => {
                if (response.status == 200) {
                  var res = response.data;
                  sourceAnnotations.clear();
                  sourceAnnotations.addFeatures(
                    sourceAnnotations
                      .getFormat()
                      .readFeatures(JSON.stringify(res))
                  );
                }
              });
            },
            strategy: bbox,
          });

          var styleFunction = function (feature) {
            console.log(feature);
            //now you can use any property of your feature to identify the different colors
            //I am using the ID property of your data just to demonstrate
            var color = feature.get("strokeColor")
              ? feature.get("strokeColor")
              : self.featureColor;

            var retStyle = new Style({
              stroke: new Stroke({
                color: color,
                width: 2,
              }),
              fill: new Fill({
                color: "rgba(255, 255, 255, 0.2)",
              }),
            });
            return retStyle;
          };
          var vectorAnnotations = new VectorLayer({
            name: "Vector",
            source: sourceAnnotations,
            style: styleFunction,
          });

          var map = new Map({
            target: "mapOL",
            layers: [baseLayer, vector, vectorAnnotations],
            view: view,
          });
          self.dataMap = map;

          var modify = new Modify({ source: sourceAnnotations });
          map.addInteraction(modify);
          var draw, snap; // global so we can remove them later
          var typeSelect = document.getElementById("type");

          function addInteractions() {
            draw = new Draw({
              source: sourceAnnotations,
              type: typeSelect.value,
            });
            draw.on("drawend", drawEnd);
            map.addInteraction(draw);
            snap = new Snap({ source: sourceAnnotations });
            map.addInteraction(snap);
          }

          function drawEnd(e) {
            var writer = new GeoJSON();
            e.feature.set("strokeColor", self.featureColor);
            var features = sourceAnnotations.getFeatures();
            features = features.concat(e.feature);
            var geojsonStr = writer.writeFeatures(features);
            console.log(geojsonStr);
            var url =
              constants.API_CORE_HOST + "/api/annotations/1/1/" + serId + "/";

            axios
              .post(url, JSON.parse(geojsonStr))
              .then((response) => console.log(response));
          }

          typeSelect.onchange = function () {
            map.removeInteraction(draw);
            map.removeInteraction(snap);
            addInteractions();
          };

          addInteractions();
        });
    },
  },
};
</script>
<style>
@import "~ol/ol.css";

#mapOL {
  height: 600px;
}
</style>
