<template>
  <div>
    <b-form action="#" @submit="onSubmit">
      <b-form-group
          class="my-3"
          label-for="plate"
          label="Boat license plate"
          description="Use the pattern AB1234"
      >
        <b-form-input
            id="plate"
            placeholder="AB1234"
            v-model="form.plate"
            pattern="[A-Za-z]{2}[0-9]{2,6}"
            required>
        </b-form-input>
      </b-form-group>
      <div class="my-2">
        <b-button type="submit" variant="success" :disabled="watchId!=null">Enable</b-button>
        <b-button class="mx-3" variant="danger" v-on:click="disableGPS" :disabled="watchId==null">Disable</b-button>
      </div>
    </b-form>
    <div class="text-center" v-if="watchId!=null">
      <h3 class="text-success">Sending device position...</h3>
      <p>Keep this page opened until the dock operation</p>
    </div>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: "GPS",
  data() {
    return {
      form: {
        plate: '',
      },
      watchId: null,
    }
  },
  mounted() {
    if (!("geolocation" in navigator)) {
      const errorStr = 'Geolocation is not available.';
      alert(errorStr)
      return;
    }
  },
  methods: {
    onSubmit(event) {
      event.preventDefault();
      const options = {
        enableHighAccuracy: true,
        maximumAge: 1000
      }
      this.watchId = navigator.geolocation.watchPosition(pos => {
        this.sendGPSUpdate(pos.coords.latitude, pos.coords.longitude)
      }, err => {
        console.log(err.message)
      }, options)
      return false;
    },
    disableGPS() {
      if (this.watchId) {
        navigator.geolocation.clearWatch(this.watchId);
        this.watchId = null
      }
    },
    sendGPSUpdate(lat, long) {
      console.log("POS", lat, long)
      const data = {}
      const headers = {
        headers: {
          "boat-id": this.form.plate,
          "latitude": lat,
          "longitude": long
        }
      }
      axios.post("https://kayib6g5eg.execute-api.us-east-1.amazonaws.com/default/GPSHandle", data, headers).then(d => {
        console.log(d.data)
      }).catch(e => {
        console.log(e)
      })
    }
  }
}
</script>

<style scoped>
.checkboxText {
  padding-left: 10px;
}
</style>