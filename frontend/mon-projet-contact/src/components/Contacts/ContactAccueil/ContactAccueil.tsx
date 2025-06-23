

const ContactAccueil = () => {
  return (
    
      <div className="flex-1 space-y-6">
          <h1 className="text-3xl md:text-5xl font-bold">
            Le compte qui <span className="text-blue-600">simplifie</span>
            votre gestion <span className="text-indigo-600">de contacts</span>
          </h1>
          <p className="text-gray-600 text-lg">
            Centralisez, organisez et accédez à tous vos contacts professionnels et personnels depuis une seule interface claire et intuitive. Gagnez du temps et restez connecté avec ceux qui comptent.
          </p>

          <div className="flex gap-4 flex-wrap">
            <button className="bg-gradient-to-r w-[300px] from-blue-500 to-purple-600 text-white px-6 py-3  rounded-full shadow-lg hover:opacity-90">
              Démarrer →
            </button>
            <button className="bg-white border w-[300px] border-blue-300 text-blue-600 px-6 py-3 rounded-full hover:bg-blue-50">
              Voir les contacts →
            </button>
          </div>
    </div>
  )
}

export default ContactAccueil
