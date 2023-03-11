const express = require('express');
const axios = require('axios');
const app = express();

app.set('view engine', 'ejs');
app.use(express.static('public'));

app.get('/', (req, res) => {
    Promise.all([
      axios.get('http://192.168.4.1/pressure1'),
      axios.get('http://192.168.4.1/pressure2'),
      axios.get('http://192.168.4.1/person1'),
      axios.get('http://192.168.4.1/person2')
    ])
    .then(([response1, response2, response3, response4]) => {
      const data1 = response1.data;
      const data2 = response2.data;
      const data3 = response3.data;
      const data4 = response4.data;
      res.render('index', { pressure1: data1, pressure2: data2, person1: data3, person2: data4 });
    })
    .catch(error => {
      console.error(error);
      res.status(500).send('Error retrieving data');
    });
});

const server = app.listen(8000, () => {
  console.log(`Server running on port ${server.address().port}`);
});
