import { useNavigate } from "react-router-dom";
import { useState } from "react";
import type { ChangeEvent, FormEvent } from "react";


interface FormulaireContact {
  nom: string;
  tel: string;
  mail: string;
  prenom: string;
  entreprise: string;
  adresse: string;
  secteur: string;
  texte: string;
  accepter: boolean;
  code: string;
  image: File | null;
}

export default function ContactForm() {
  const navigate = useNavigate();

  const [formData, setFormData] = useState<FormulaireContact>({
    nom: "",
    tel: "",
    mail: "",
    prenom: "",
    entreprise: "",
    adresse: "",
    secteur: "",
    texte: "",
    accepter: false,
    code: "",
    image: null,
  });

  const handleChange = (e: ChangeEvent<HTMLInputElement | HTMLTextAreaElement | HTMLSelectElement>) => {
    const { name, value, type } = e.target;

    if (type === "checkbox") {
      setFormData({ ...formData, [name]: (e.target as HTMLInputElement).checked });
    } else if (type === "file") {
      setFormData({ ...formData, image: (e.target as HTMLInputElement).files?.[0] || null });
    } else {
      setFormData({ ...formData, [name]: value });
    }
  };

  const handleSubmit = async (e: FormEvent) => {
    e.preventDefault();

    const contactToSend = {
      nom: formData.nom,
      tel: formData.tel,
      mail: formData.mail,
    };

    try {
      const res = await fetch("http://localhost:8888/contacts", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(contactToSend),
      });

      if (res.ok) {
        alert("✅ Contact ajouté !");
        navigate("/accueil");
      } else {
        alert("❌ Échec de l'ajout.");
      }
    } catch (err) {
      alert("❌ Erreur de connexion.");
      console.error(err);
    }
  };

  

  return (
    <div className="min-h-screen w-full bg-gradient-to-br from-green-100 via-white to-blue-100 flex flex-col">
      <div className="flex justify-between items-center p-6 max-w-6xl mx-auto">
        <form onSubmit={handleSubmit} className="w-full">
          <h2 className="text-2xl font-bold text-indigo-600 mb-4">Formulaire de contact..</h2>

          {/* Nom complet */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Nom complet *</label>
            <div className="flex gap-2">
              <input type="text" name="prenom" placeholder="Prénom" value={formData.prenom} onChange={handleChange} className="border w-1/2 p-2" />
              <input type="text" name="nom" placeholder="Nom de famille" value={formData.nom} onChange={handleChange} className="border w-1/2 p-2" required />
            </div>
          </div>

          {/* Entreprise */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Entreprise *</label>
            <input type="text" name="entreprise" className="border w-full p-2" value={formData.entreprise} onChange={handleChange} required />
            <p className="text-red-600 mt-1 text-sm">❗Ce champ est requis.</p>
          </div>

          {/* Adresse */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Adresse *</label>
            <input type="text" name="adresse" className="border w-full p-2" value={formData.adresse} onChange={handleChange} />
          </div>

          {/* E-mail */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">E-mail *</label>
            <input type="email" name="mail" placeholder="ex: myname@example.com" value={formData.mail} onChange={handleChange} className="border w-full p-2" required />
          </div>

          {/* Téléphone */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Numéro de téléphone *</label>
            <div className="flex gap-2">
              <input type="text" placeholder="Indicatif" className="border w-1/4 p-2" />
              <input type="text" name="tel" placeholder="Numéro de téléphone" value={formData.tel} onChange={handleChange} className="border w-3/4 p-2" required />
            </div>
          </div>

          {/* Secteur */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Secteur de l’entreprise *</label>
            <select name="secteur" value={formData.secteur} onChange={handleChange} className="border w-full p-2">
              <option value="">Veuillez sélectionner</option>
              <option>Informatique</option>
              <option>Construction</option>
              <option>Éducation</option>
            </select>
          </div>

          {/* Image */}
          <div className="mb-4">
            <label>Image</label><br />
            <input name="image" type="file" onChange={handleChange} className="rounded-full" />
          </div>

          {/* Zone de texte */}
          <div className="mb-4">
            <label className="block mb-1 font-semibold">Zone de texte *</label>
            <textarea name="texte" rows={4} value={formData.texte} onChange={handleChange} className="border w-full p-2" />
          </div>

          {/* RGPD */}
          <div className="text-xs text-gray-700 leading-snug mb-2">
            <p>
              Traitement des données personnelles... En acceptant, vous autorisez l'utilisation de ces données.
            </p>
          </div>

          {/* Acceptation */}
          <div className="flex items-center gap-2 mb-4">
            <input type="checkbox" name="accepter" checked={formData.accepter} onChange={handleChange} />
            <label>J’accepte. <span className="text-red-600">*</span></label>
          </div>

          {/* Boutons */}
          <div className="flex items-center gap-4 flex-wrap my-6">
            <button type="submit" className="bg-gradient-to-r w-[170px] from-blue-500 to-purple-600 text-white px-6 py-3 rounded-full shadow-lg hover:opacity-90">
              Enregistrer ✓
            </button>
            <button type="button" onClick={()=>navigate('/contact/')} className="bg-white border w-[170px] border-blue-300 text-blue-600 px-6 py-3 rounded-full hover:bg-blue-50">
              Retour 🔙
            </button>
          </div>
        </form>
      </div>
    </div>
  );
}
