<template>
  <div>
    <h2>Reserve a docking spot</h2>
    <p>Insert here your information</p>
    <b-form action="#" @submit="onSubmit" @reset="onReset">

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

      <b-form-group class="my-3"
                    label="Boat size category:" label-for="boatSize">
        <b-form-select
            class="form-select"
            id="boatSize"
            v-model="form.sizeCategory"
            :options="sizeCategories"
            required
        ></b-form-select>
      </b-form-group>

      <b-form-group
          class="my-3"
          label-for="ownerName"
          label="Owner full name">
        <b-form-input
            id="ownerName"
            placeholder="Mario Rossi"
            v-model="form.ownerName"
            required>
        </b-form-input>
      </b-form-group>

      <div class="my-2">
        <b-button type="submit" variant="primary">Submit</b-button>
        <b-button class="mx-3" type="reset" variant="warning">Reset</b-button>
      </div>

    </b-form>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: "Reservation",
  data() {
    return {
      form: {
        ownerName: '',
        plate: '',
        sizeCategory: null
      },
      sizeCategories: [
        {text: 'Select One', value: null},
        {text: 'Until 7.99 meters', value: 1},
        {text: 'From 8 to 12.99 meters', value: 2},
        {text: 'From 13 to 17 meters', value: 3}],
    }
  },
  methods: {
    onSubmit() {
      const data = {}
      const headers = {
        headers: {
          "boat-id": this.form.plate,
          "boat-dim": this.form.sizeCategory,
          "owner": this.form.ownerName
        }
      }

      axios.post("https://zki9fxvfvi.execute-api.us-east-1.amazonaws.com/default/getReservation", data, headers)
          .then(d => {
            if (d.data.ret_code == 0) {
              this.onReset();
              alert(d.data.info_mess)
            } else
              alert(d.data.info_mess)
          })
          .catch(e => {
            console.log(e);
          })
      return false;
    },
    onReset() {
      this.form.ownerName = ""
      this.form.plate = ""
      this.sizeCategory = null
    }
  }
}
</script>

<style scoped>

</style>