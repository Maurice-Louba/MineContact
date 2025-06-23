
import './App.css'
import ToutAffichage from './components/Affichage/ToutAffichage'
import Toutcontact from './components/Contacts/Toutcontact'
import Inscription from './components/Inscription/Inscription'

import Tout from './components/tout'
import { BrowserRouter as Router, Routes,Route } from 'react-router'

function App() {
  return (
    <Router>
      <Routes>
        <Route path='' element={<Tout />}/>
        <Route path='contact/' element={<Toutcontact/>}/>
        <Route path='contact/Formulaire/' element={<Inscription/>}/>
        <Route path='contact/liste/' element={<ToutAffichage/>}/>
      </Routes>
    </Router>
  )
}

export default App

