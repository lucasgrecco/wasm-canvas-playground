const PORT = process.env.PORT || 5000
const path = require('path')

// Express
const express = require('express')
const app = express()

app.use(express.static(path.join(__dirname, 'public'),{
      setHeaders: (res, path, stat) => {
        // Serve .wasm files with correct mime type
        if (path.endsWith('.wasm')) {
          res.set('Content-Type', 'application/wasm')
        }
      }
    }
  ))
  .get('/', (req, res) => res.render('public/index'))
  .listen(PORT, () => console.log(`localhost:${PORT}`))