#ifndef SDHCALRPC_h
#define SDHCALRPC_h

#include <globals.hh>
#include <G4Material.hh>

#include <G4VPhysicalVolume.hh>
#include <G4AffineTransform.hh>

#include <G4VSensitiveDetector.hh>
#include <G4THitsCollection.hh>

#include <vector>
#include <set>

class G4LogicalVolume ;
class SDHCALRPCSensitiveDetector ;

struct SDHCALRPCGeom
{
	G4int nPadX {} ;
	G4int nPadY {} ;
	G4double cellSize {} ;
	
	struct Layer
	{
		G4String name ;
		G4double width ; //in mm
		G4String material ;
	} ;

	std::vector<Layer> layers {} ;
} ;

class SDHCALHit ;
typedef G4THitsCollection<SDHCALHit> SDHCALHitCollection ;

class SDHCALRPC : public G4VSensitiveDetector
{
	public : 
		static SDHCALRPC* buildStandardRPC(G4int _id , G4int _nPadX , G4int _nPadY , G4double _cellSize) ;
		static SDHCALRPC* buildOldStandardRPC(G4int _id , G4int _nPadX , G4int _nPadY , G4double _cellSize) ;
		static SDHCALRPC* buildWithScintillatorRPC(G4int _id , G4int _nPadX , G4int _nPadY , G4double _cellSize) ;

	public :
		SDHCALRPC(G4int _id , const SDHCALRPCGeom& _geom) ;
		virtual ~SDHCALRPC() = default ;

		G4LogicalVolume* getLogicRPC() { return logicRPC ; }
		G4VPhysicalVolume* getPhysicRPC() { return physicRPC ; }

		inline G4int getnPadX() const { return nPadX ; }
		inline G4int getnPadY() const { return nPadY ; }
		inline G4double getCellSize() const { return cellSize ; }
		inline G4double getSizeX() const { return sizeX ; }
		inline G4double getSizeY() const { return sizeY ; }
		inline G4double getSizeZ() const { return sizeZ ; }
		inline G4int getID() const { return id ; }

		inline G4bool isTransformComputed() const { return transformComputed ; }
		void setCoordTransform(G4AffineTransform trans) ;
		inline const G4AffineTransform& getGlobalToRpcTransform() const { return globalToRpcTransform ; }


		const G4ThreeVector globalToRpcCoordTransform(G4ThreeVector globalCoord) const ;
		const G4ThreeVector rpcToGlobalCoordTransform(G4ThreeVector localCoord) const ;

		const G4ThreeVector IJToLocalCoord(G4int I , G4int J) const ;
		std::vector<int> localCoordToIJ(G4ThreeVector localCoord) const ;

		G4ThreeVector getGlobalCoord(G4int I , G4int J) const ;

		G4VPhysicalVolume* createPhysicalVolume(G4RotationMatrix* rot , G4ThreeVector trans , G4LogicalVolume* motherLogic) ;

		SDHCALRPC(const SDHCALRPC&) = delete ;
		void operator=(const SDHCALRPC&) = delete ;


		virtual void Initialize(G4HCofThisEvent*) ;
		virtual G4bool ProcessHits(G4Step* step , G4TouchableHistory*) ;

		virtual void EndOfEvent(G4HCofThisEvent*) ;

		virtual G4bool Interested(const G4Step* step) const ;

		void finalizeLastHit() ;


	protected :

		virtual void build(const SDHCALRPCGeom& _geom) ;

		// G4String name {} ;

		G4int id {} ;
		G4int nPadX {} ;
		G4int nPadY {} ;
		G4double cellSize {} ;
		G4double sizeX {} ;
		G4double sizeY {} ;
		G4double sizeZ {} ;

		G4bool transformComputed {} ;
		G4AffineTransform rpcToGlobalTransform {} ;
		G4AffineTransform globalToRpcTransform {} ;

		G4LogicalVolume* logicRPC {} ;
		G4VPhysicalVolume* physicRPC {} ;
		// SDHCALRPCSensitiveDetector* sensitiveDetector {} ;

		G4VPhysicalVolume* physiGasGap {} ;

		SDHCALHitCollection* hitsCollection {} ;

		SDHCALHit* currentHit {} ;
} ;



#endif //SDHCALRPC_h
